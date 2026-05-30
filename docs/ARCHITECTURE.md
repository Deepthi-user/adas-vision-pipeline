# ADAS Vision Pipeline - Architecture

## System Overview

The ADAS Vision Pipeline is designed as a modular, multithreaded system for real-time video processing with focus on performance and maintainability.

## Core Components

### 1. Camera Module (`camera/`)
- Handles video input from files or webcam
- Frame synchronization and timestamping
- Resolution and FPS management

**Key Classes:**
- `CameraDriver`: Main camera interface

### 2. Detection Module (`detection/`)
- **Lane Detection**: Hough transform-based lane boundary detection
- **Vehicle Detection**: YOLO-based vehicle detection with bounding boxes
- **Sign Detection**: CNN-based traffic sign classification

**Key Classes:**
- `LaneDetector`: Lane detection pipeline
- `VehicleDetector`: YOLO vehicle detection
- `SignDetector`: Traffic sign recognition

### 3. Tracking Module (`tracking/`)
- Multi-object tracking using Kalman filters
- Track association and management
- Track lifecycle management

**Key Classes:**
- `ObjectTracker`: Main tracking engine
- `KalmanFilter`: State estimation filter

### 4. Calibration Module (`calibration/`)
- Camera intrinsic parameter calibration
- Lens distortion correction
- Calibration persistence

**Key Classes:**
- `Calibrator`: Camera calibration manager

### 5. Visualization Module (`visualization/`)
- Real-time frame display
- Bounding box rendering
- Lane overlay visualization
- FPS counter display
- Video output recording

**Key Classes:**
- `Visualizer`: Rendering and display

### 6. Benchmarks Module (`benchmarks/`)
- FPS measurement and tracking
- Performance profiling
- Bottleneck identification

**Key Classes:**
- `FPSCounter`: Frame rate measurement
- `BenchmarkSuite`: Performance testing

## Data Flow

```
┌─────────────┐
│   Camera    │  (Input: Video/Webcam)
└──────┬──────┘
       │
       ▼
┌──────────────────┐
│  Preprocessing   │  (Undistortion, Resizing)
└──────┬───────────┘
       │
       ├─────────────────┬────────────────┬─────────────┐
       │                 │                │             │
       ▼                 ▼                ▼             ▼
   ┌────────┐      ┌──────────┐     ┌────────┐   ┌─────────┐
   │  Lane  │      │ Vehicle  │     │  Sign  │   │Tracking │
   │Detector│      │ Detector │     │Detector│   │         │
   └────┬───┘      └──────┬───┘     └────┬───┘   └────┬────┘
        │                 │             │            │
        └─────────────────┼─────────────┴────────────┘
                          │
                          ▼
                  ┌──────────────┐
                  │ Visualizer   │  (Output: Display + Video)
                  └──────────────┘
                          │
                          ▼
                  ┌──────────────┐
                  │ FPS Counter  │  (Metrics)
                  └──────────────┘
```

## Threading Model

The pipeline uses producer-consumer pattern with thread pools:

```
Main Thread
    ├─ Camera Input Thread
    ├─ Detection Thread Pool
    │   ├─ Lane Detection Worker
    │   ├─ Vehicle Detection Worker
    │   └─ Sign Detection Worker
    ├─ Tracking Thread
    ├─ Visualization Thread
    └─ Metrics Thread
```

## Performance Characteristics

### Target FPS
- Lane Detection: 30+ FPS @ 1280x720
- Vehicle Detection: 25+ FPS @ 1280x720
- Overall Pipeline: 20+ FPS @ 1280x720

### Optimization Techniques
1. **Frame Skipping**: Lower resolution for non-critical frames
2. **ROI Processing**: Limit detection to regions of interest
3. **Parallel Processing**: Multi-threaded pipeline
4. **Model Optimization**: Quantized YOLO models

## Design Patterns

1. **Producer-Consumer**: Frame queue between camera and processors
2. **Thread Pool**: Parallel detection workers
3. **Kalman Filter**: Temporal filtering for tracking
4. **Pipeline Pattern**: Sequential module processing

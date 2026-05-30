# ADAS Vision Pipeline

A high-performance Advanced Driver Assistance System (ADAS) vision pipeline built with C++ and OpenCV, featuring real-time lane detection, vehicle detection, and traffic sign recognition.

## 🎯 Features

### Core Capabilities
- **Lane Detection**: Real-time lane boundary detection using Hough transforms and deep learning
- **Vehicle Detection**: YOLO-based vehicle detection with bounding box tracking
- **Traffic Sign Recognition**: CNN-based traffic sign classification
- **Distance Estimation**: 3D position estimation from 2D detection
- **Real-time Processing**: Multithreaded video pipeline with FPS measurement

## 🛠️ Technology Stack

- **Language**: C++17
- **Vision Library**: OpenCV 4.x
- **Deep Learning**: YOLO (PyTorch/ONNX)
- **Threading**: std::thread with thread pools
- **Build System**: CMake 3.16+

## 📁 Project Structure

```
adas-vision-pipeline/
├── camera/                    # Camera input and calibration
│   ├── camera_driver.cpp
│   ├── camera_driver.h
│   └── CMakeLists.txt
├── detection/                 # Detection modules
│   ├── lane_detector.cpp
│   ├── lane_detector.h
│   ├── vehicle_detector.cpp
│   ├── vehicle_detector.h
│   ├── sign_detector.cpp
│   ├── sign_detector.h
│   └── CMakeLists.txt
├── tracking/                  # Object tracking
│   ├── object_tracker.cpp
│   ├── object_tracker.h
│   ├── kalman_filter.cpp
│   ├── kalman_filter.h
│   └── CMakeLists.txt
├── calibration/               # Camera calibration
│   ├── calibrator.cpp
│   ├── calibrator.h
│   └── CMakeLists.txt
├── visualization/             # Visualization and output
│   ├── visualizer.cpp
│   ├── visualizer.h
│   └── CMakeLists.txt
├── benchmarks/                # Performance benchmarks
│   ├── benchmark_suite.cpp
│   ├── fps_counter.cpp
│   ├── fps_counter.h
│   └── CMakeLists.txt
├── docs/                      # Documentation
│   ├── ARCHITECTURE.md
│   ├── SETUP.md
│   └── API.md
├── CMakeLists.txt
├── main.cpp
└── .gitignore
```

## 🚀 Quick Start

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt-get install libopencv-dev cmake g++ libpthread-stubs0-dev

# macOS
brew install opencv cmake
```

### Build
```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### Run
```bash
./adas_pipeline --video input.mp4
# or for webcam
./adas_pipeline --camera 0
```

## ✨ Key Features 

✅ **Multithreading**: Producer-consumer pattern with thread pools for parallel processing
✅ **Video Pipeline**: Efficient frame-by-frame processing with queue management
✅ **FPS Measurement**: Real-time performance monitoring and bottleneck analysis
✅ **Modular Architecture**: Loosely coupled components with clear interfaces
✅ **Optimization**: SIMD operations, frame skipping, adaptive resolution scaling

## 📊 Performance Targets

- **Lane Detection**: 30+ FPS @ 1280x720
- **Vehicle Detection**: 25+ FPS @ 1280x720 (YOLO)
- **Overall Pipeline**: 20+ FPS @ 1280x720 (all modules combined)

## 📚 Documentation

- [Architecture Overview](docs/ARCHITECTURE.md)
- [Setup Guide](docs/SETUP.md)
- [API Reference](docs/API.md)

## 🤝 Contributing

Contributions are welcome! Please ensure:
- Code follows C++17 standards
- Include unit tests for new features
- Update documentation as needed
- Run benchmarks before and after changes

## 📝 License

MIT License - See LICENSE file for details

## 👤 Author

Deepthi-user

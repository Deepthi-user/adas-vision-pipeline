# ADAS Vision Pipeline - API Reference

## Camera Module

### CameraDriver

```cpp
class CameraDriver {
public:
    struct Frame {
        cv::Mat image;        // Image data
        uint64_t timestamp;   // Timestamp in ticks
        uint32_t frame_id;    // Frame sequence number
    };

    // Initialization
    bool initializeFromFile(const std::string& video_path);
    bool initializeFromCamera(int camera_index = 0);

    // Frame access
    bool getFrame(Frame& frame);

    // Properties
    int getWidth() const;      // Frame width
    int getHeight() const;     // Frame height
    double getFPS() const;     // Frames per second
    int getTotalFrames() const; // Total frame count

    // Control
    void pause();
    void resume();
    void reset();
    bool isRunning() const;
};
```

## Detection Module

### LaneDetector

```cpp
class LaneDetector {
public:
    struct Lane {
        std::vector<cv::Point> left_points;
        std::vector<cv::Point> right_points;
        double left_angle;
        double right_angle;
        double lane_width;
    };

    bool detect(const cv::Mat& image, Lane& lane);
    void setThresholds(int lower_hue, int upper_hue);
    void setCannyThresholds(int threshold1, int threshold2);
    cv::Mat getDebugImage() const;
};
```

### VehicleDetector

```cpp
class VehicleDetector {
public:
    struct Detection {
        cv::Rect bbox;         // Bounding box
        float confidence;      // Confidence score [0, 1]
        int class_id;          // Class identifier
        std::string class_name; // Class name
    };

    bool detect(const cv::Mat& image, std::vector<Detection>& detections);
    void setConfidenceThreshold(float threshold);
    double getInferenceFPS() const;
};
```

### SignDetector

```cpp
class SignDetector {
public:
    struct Sign {
        cv::Rect bbox;              // Bounding box
        std::string class_name;     // Sign type
        float confidence;           // Confidence score
    };

    bool detect(const cv::Mat& image, std::vector<Sign>& signs);
    void setConfidenceThreshold(float threshold);
};
```

## Tracking Module

### ObjectTracker

```cpp
class ObjectTracker {
public:
    struct TrackedObject {
        int track_id;              // Unique track ID
        cv::Rect bbox;             // Current bounding box
        cv::Point2f center;        // Object center
        std::shared_ptr<KalmanFilter> kf;  // Kalman filter
        int frame_count;           // Frames without update
        int age;                   // Track age
    };

    void update(const std::vector<cv::Rect>& detections);
    std::vector<TrackedObject> getTrackedObjects() const;
    void setMaxDistance(float distance);
    void setMinFrames(int frames);
};
```

### KalmanFilter

```cpp
class KalmanFilter {
public:
    cv::Mat predict();
    void update(const cv::Mat& measurement);
    cv::Mat getState() const;
};
```

## Calibration Module

### Calibrator

```cpp
class Calibrator {
public:
    struct CalibrationResult {
        cv::Mat camera_matrix;     // 3x3 intrinsic matrix
        cv::Mat dist_coeffs;       // Distortion coefficients
        double reprojection_error; // Calibration error
    };

    bool calibrateFromImages(const std::vector<std::string>& image_paths,
                            cv::Size checkerboard_size,
                            CalibrationResult& result);
    cv::Mat undistortImage(const cv::Mat& image);
    bool saveCalibration(const std::string& filepath);
    bool loadCalibration(const std::string& filepath);
};
```

## Visualization Module

### Visualizer

```cpp
class Visualizer {
public:
    void displayFrame(const cv::Mat& frame);
    void drawBoundingBox(cv::Mat& image, const cv::Rect& bbox,
                        const std::string& label,
                        const cv::Scalar& color = cv::Scalar(0, 255, 0),
                        int thickness = 2);
    void drawLanes(cv::Mat& image, const std::vector<cv::Point>& left_lane,
                   const std::vector<cv::Point>& right_lane);
    void drawFPS(cv::Mat& image, double fps);
    bool saveFrame(const cv::Mat& frame, const std::string& filename);
    bool initializeVideoWriter(const std::string& output_path, int fps);
    void writeFrame(const cv::Mat& frame);
};
```

## Benchmarks Module

### FPSCounter

```cpp
class FPSCounter {
public:
    FPSCounter(int window_size = 30);
    void tick();                   // Call once per frame
    double getFPS() const;         // Current FPS
    double getAverageFPS() const;  // Average FPS
    double getFrameTime() const;   // Frame time in ms
    void reset();
};
```

## Example Usage

```cpp
#include "camera/camera_driver.h"
#include "detection/lane_detector.h"
#include "detection/vehicle_detector.h"
#include "tracking/object_tracker.h"
#include "visualization/visualizer.h"
#include "benchmarks/fps_counter.h"

int main() {
    // Initialize
    CameraDriver camera;
    camera.initializeFromFile("input.mp4");

    LaneDetector lane_detector(camera.getHeight(), camera.getWidth());
    VehicleDetector vehicle_detector("models/yolov3.weights");
    ObjectTracker tracker;
    Visualizer visualizer(camera.getWidth(), camera.getHeight());
    FPSCounter fps_counter;

    // Main loop
    CameraDriver::Frame frame;
    while (camera.getFrame(frame)) {
        // Detect lanes
        LaneDetector::Lane lane;
        lane_detector.detect(frame.image, lane);

        // Detect vehicles
        std::vector<VehicleDetector::Detection> detections;
        vehicle_detector.detect(frame.image, detections);

        // Extract bounding boxes for tracking
        std::vector<cv::Rect> bboxes;
        for (const auto& det : detections) {
            bboxes.push_back(det.bbox);
        }
        tracker.update(bboxes);

        // Visualize
        cv::Mat output = frame.image.clone();
        visualizer.drawLanes(output, lane.left_points, lane.right_points);
        for (const auto& det : detections) {
            visualizer.drawBoundingBox(output, det.bbox, det.class_name);
        }
        visualizer.drawFPS(output, fps_counter.getFPS());
        visualizer.displayFrame(output);

        fps_counter.tick();
    }

    return 0;
}
```

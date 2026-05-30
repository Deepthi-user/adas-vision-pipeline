#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class VehicleDetector {
public:
    struct Detection {
        cv::Rect bbox;
        float confidence;
        int class_id;
        std::string class_name;
    };

    VehicleDetector(const std::string& model_path);
    ~VehicleDetector();

    // Detect vehicles in image
    bool detect(const cv::Mat& image, std::vector<Detection>& detections);

    // Set confidence threshold
    void setConfidenceThreshold(float threshold) { confidence_threshold_ = threshold; }

    // Get FPS
    double getInferenceFPS() const { return inference_fps_; }

private:
    cv::dnn::Net net_;
    std::vector<std::string> class_names_;
    float confidence_threshold_ = 0.5f;
    double inference_fps_ = 0.0;
    int input_width_ = 416;
    int input_height_ = 416;
};

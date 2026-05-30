#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class SignDetector {
public:
    struct Sign {
        cv::Rect bbox;
        std::string class_name;
        float confidence;
    };

    SignDetector(const std::string& model_path);
    ~SignDetector();

    // Detect traffic signs
    bool detect(const cv::Mat& image, std::vector<Sign>& signs);

    // Set confidence threshold
    void setConfidenceThreshold(float threshold) { confidence_threshold_ = threshold; }

private:
    cv::dnn::Net net_;
    float confidence_threshold_ = 0.6f;
    int input_size_ = 32;
};

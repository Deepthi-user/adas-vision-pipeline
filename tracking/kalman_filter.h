#pragma once

#include <opencv2/opencv.hpp>

class KalmanFilter {
public:
    KalmanFilter();
    ~KalmanFilter();

    // Predict next state
    cv::Mat predict();

    // Update with measurement
    void update(const cv::Mat& measurement);

    // Get current state
    cv::Mat getState() const { return state_; }

private:
    cv::KalmanFilter kf_;
    cv::Mat state_;
    cv::Mat measurement_;
};

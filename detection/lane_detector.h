#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <memory>

class LaneDetector {
public:
    struct Lane {
        std::vector<cv::Point> left_points;
        std::vector<cv::Point> right_points;
        double left_angle;
        double right_angle;
        double lane_width;
    };

    LaneDetector(int img_height, int img_width);
    ~LaneDetector();

    // Detect lanes in image
    bool detect(const cv::Mat& image, Lane& lane);

    // Set parameters
    void setThresholds(int lower_hue, int upper_hue);
    void setCannyThresholds(int threshold1, int threshold2);

    // Get debug visualization
    cv::Mat getDebugImage() const { return debug_image_; }

private:
    int img_height_;
    int img_width_;
    cv::Mat debug_image_;

    // Helper methods
    cv::Mat preprocessImage(const cv::Mat& image);
    void detectLineEdges(const cv::Mat& image, std::vector<cv::Vec4i>& lines);
    void separateLanes(const std::vector<cv::Vec4i>& lines, Lane& lane);
};

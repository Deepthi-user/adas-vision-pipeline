#include "lane_detector.h"
#include <iostream>

LaneDetector::LaneDetector(int img_height, int img_width)
    : img_height_(img_height), img_width_(img_width) {}

LaneDetector::~LaneDetector() {}

bool LaneDetector::detect(const cv::Mat& image, Lane& lane) {
    if (image.empty()) {
        return false;
    }

    // Preprocess image
    cv::Mat processed = preprocessImage(image);

    // Detect edges
    std::vector<cv::Vec4i> lines;
    detectLineEdges(processed, lines);

    if (lines.empty()) {
        return false;
    }

    // Separate left and right lanes
    separateLanes(lines, lane);

    // Store debug image
    debug_image_ = image.clone();
    cv::polylines(debug_image_, {lane.left_points}, false, cv::Scalar(0, 255, 0), 2);
    cv::polylines(debug_image_, {lane.right_points}, false, cv::Scalar(0, 0, 255), 2);

    return true;
}

void LaneDetector::setThresholds(int lower_hue, int upper_hue) {
    // Threshold configuration for color-based lane detection
}

void LaneDetector::setCannyThresholds(int threshold1, int threshold2) {
    // Canny edge detection threshold configuration
}

cv::Mat LaneDetector::preprocessImage(const cv::Mat& image) {
    cv::Mat gray, blurred, edges;

    // Convert to grayscale
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Blur to reduce noise
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);

    // Canny edge detection
    cv::Canny(blurred, edges, 50, 150);

    return edges;
}

void LaneDetector::detectLineEdges(const cv::Mat& image, std::vector<cv::Vec4i>& lines) {
    // Hough line transform
    cv::HoughLinesP(image, lines, 1, CV_PI / 180, 50, 50, 10);
}

void LaneDetector::separateLanes(const std::vector<cv::Vec4i>& lines, Lane& lane) {
    int mid_x = img_width_ / 2;

    for (const auto& line : lines) {
        int x1 = line[0], y1 = line[1];
        int x2 = line[2], y2 = line[3];

        if ((x1 + x2) / 2 < mid_x) {
            lane.left_points.push_back(cv::Point(x1, y1));
            lane.left_points.push_back(cv::Point(x2, y2));
        } else {
            lane.right_points.push_back(cv::Point(x1, y1));
            lane.right_points.push_back(cv::Point(x2, y2));
        }
    }
}

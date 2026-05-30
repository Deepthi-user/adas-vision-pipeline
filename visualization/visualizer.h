#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

class Visualizer {
public:
    Visualizer(int width, int height);
    ~Visualizer();

    // Display frame with detections
    void displayFrame(const cv::Mat& frame);

    // Draw bounding boxes
    void drawBoundingBox(cv::Mat& image, const cv::Rect& bbox, const std::string& label,
                        const cv::Scalar& color = cv::Scalar(0, 255, 0), int thickness = 2);

    // Draw lane detection
    void drawLanes(cv::Mat& image, const std::vector<cv::Point>& left_lane,
                   const std::vector<cv::Point>& right_lane);

    // Draw FPS
    void drawFPS(cv::Mat& image, double fps);

    // Save frame to file
    bool saveFrame(const cv::Mat& frame, const std::string& filename);

    // Video writer for output
    bool initializeVideoWriter(const std::string& output_path, int fps);
    void writeFrame(const cv::Mat& frame);

private:
    int width_;
    int height_;
    cv::VideoWriter video_writer_;
};

#include "visualizer.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Visualizer::Visualizer(int width, int height) : width_(width), height_(height) {}

Visualizer::~Visualizer() {
    if (video_writer_.isOpened()) {
        video_writer_.release();
    }
}

void Visualizer::displayFrame(const cv::Mat& frame) {
    cv::imshow("ADAS Vision Pipeline", frame);
    int key = cv::waitKey(1);
    if (key == 27) {  // ESC key
        // Signal exit
    }
}

void Visualizer::drawBoundingBox(cv::Mat& image, const cv::Rect& bbox, const std::string& label,
                                 const cv::Scalar& color, int thickness) {
    // Draw rectangle
    cv::rectangle(image, bbox, color, thickness);

    // Draw label
    int baseline = 0;
    cv::Size text_size = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);
    cv::Point text_origin(bbox.x, bbox.y - 5);

    cv::rectangle(image, cv::Point(text_origin.x, text_origin.y - text_size.height - 5),
                 cv::Point(text_origin.x + text_size.width, text_origin.y + 5),
                 color, cv::FILLED);
    cv::putText(image, label, cv::Point(text_origin.x, text_origin.y - baseline),
               cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
}

void Visualizer::drawLanes(cv::Mat& image, const std::vector<cv::Point>& left_lane,
                           const std::vector<cv::Point>& right_lane) {
    if (!left_lane.empty()) {
        cv::polylines(image, {left_lane}, false, cv::Scalar(0, 255, 0), 3);
    }
    if (!right_lane.empty()) {
        cv::polylines(image, {right_lane}, false, cv::Scalar(0, 0, 255), 3);
    }
}

void Visualizer::drawFPS(cv::Mat& image, double fps) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << "FPS: " << fps;
    std::string fps_text = oss.str();

    cv::putText(image, fps_text, cv::Point(10, 30),
               cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
}

bool Visualizer::saveFrame(const cv::Mat& frame, const std::string& filename) {
    return cv::imwrite(filename, frame);
}

bool Visualizer::initializeVideoWriter(const std::string& output_path, int fps) {
    int fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
    video_writer_.open(output_path, fourcc, fps, cv::Size(width_, height_));
    return video_writer_.isOpened();
}

void Visualizer::writeFrame(const cv::Mat& frame) {
    if (video_writer_.isOpened()) {
        video_writer_.write(frame);
    }
}

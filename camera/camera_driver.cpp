#include "camera_driver.h"
#include <iostream>

CameraDriver::CameraDriver() : current_frame_id_(0), is_running_(false), is_paused_(false) {}

CameraDriver::~CameraDriver() {
    if (capture_.isOpened()) {
        capture_.release();
    }
}

bool CameraDriver::initializeFromFile(const std::string& video_path) {
    capture_.open(video_path);
    if (!capture_.isOpened()) {
        std::cerr << "Failed to open video file: " << video_path << std::endl;
        return false;
    }

    width_ = static_cast<int>(capture_.get(cv::CAP_PROP_FRAME_WIDTH));
    height_ = static_cast<int>(capture_.get(cv::CAP_PROP_FRAME_HEIGHT));
    fps_ = capture_.get(cv::CAP_PROP_FPS);
    total_frames_ = static_cast<int>(capture_.get(cv::CAP_PROP_FRAME_COUNT));

    is_running_ = true;
    std::cout << "Video initialized: " << width_ << "x" << height_ << " @ " << fps_ << " FPS" << std::endl;
    return true;
}

bool CameraDriver::initializeFromCamera(int camera_index) {
    capture_.open(camera_index);
    if (!capture_.isOpened()) {
        std::cerr << "Failed to open camera at index: " << camera_index << std::endl;
        return false;
    }

    width_ = static_cast<int>(capture_.get(cv::CAP_PROP_FRAME_WIDTH));
    height_ = static_cast<int>(capture_.get(cv::CAP_PROP_FRAME_HEIGHT));
    fps_ = capture_.get(cv::CAP_PROP_FPS);

    is_running_ = true;
    std::cout << "Camera initialized: " << width_ << "x" << height_ << " @ " << fps_ << " FPS" << std::endl;
    return true;
}

bool CameraDriver::getFrame(Frame& frame) {
    if (!is_running_ || is_paused_) {
        return false;
    }

    cv::Mat mat;
    if (!capture_.read(mat)) {
        is_running_ = false;
        return false;
    }

    frame.image = mat;
    frame.timestamp = cv::getTickCount();
    frame.frame_id = current_frame_id_++;

    return true;
}

void CameraDriver::pause() {
    is_paused_ = true;
}

void CameraDriver::resume() {
    is_paused_ = false;
}

void CameraDriver::reset() {
    if (capture_.isOpened()) {
        capture_.set(cv::CAP_PROP_POS_FRAMES, 0);
        current_frame_id_ = 0;
    }
}

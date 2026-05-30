#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <memory>
#include <queue>
#include <thread>
#include <mutex>

class CameraDriver {
public:
    struct Frame {
        cv::Mat image;
        uint64_t timestamp;
        uint32_t frame_id;
    };

    CameraDriver();
    ~CameraDriver();

    // Initialize from video file or camera index
    bool initializeFromFile(const std::string& video_path);
    bool initializeFromCamera(int camera_index = 0);

    // Get next frame
    bool getFrame(Frame& frame);

    // Camera properties
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    double getFPS() const { return fps_; }
    int getTotalFrames() const { return total_frames_; }

    // Control
    void pause();
    void resume();
    void reset();
    bool isRunning() const { return is_running_; }

private:
    cv::VideoCapture capture_;
    int width_ = 0;
    int height_ = 0;
    double fps_ = 0.0;
    int total_frames_ = 0;
    uint32_t current_frame_id_ = 0;
    bool is_running_ = false;
    bool is_paused_ = false;
};

#pragma once

#include <deque>
#include <chrono>

class FPSCounter {
public:
    FPSCounter(int window_size = 30);
    ~FPSCounter();

    // Mark a frame as processed
    void tick();

    // Get current FPS
    double getFPS() const { return fps_; }

    // Get average FPS
    double getAverageFPS() const { return avg_fps_; }

    // Get frame time in ms
    double getFrameTime() const { return frame_time_ms_; }

    // Reset counter
    void reset();

private:
    std::deque<std::chrono::high_resolution_clock::time_point> timestamps_;
    int window_size_;
    double fps_ = 0.0;
    double avg_fps_ = 0.0;
    double frame_time_ms_ = 0.0;
};

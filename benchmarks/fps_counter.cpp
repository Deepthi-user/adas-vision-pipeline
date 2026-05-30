#include "fps_counter.h"
#include <algorithm>

FPSCounter::FPSCounter(int window_size) : window_size_(window_size) {}

FPSCounter::~FPSCounter() {}

void FPSCounter::tick() {
    auto now = std::chrono::high_resolution_clock::now();
    timestamps_.push_back(now);

    if (timestamps_.size() > static_cast<size_t>(window_size_)) {
        timestamps_.pop_front();
    }

    if (timestamps_.size() >= 2) {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            timestamps_.back() - timestamps_.front());
        double time_sec = duration.count() / 1000.0;
        fps_ = (timestamps_.size() - 1) / time_sec;
    }

    // Calculate average FPS
    if (timestamps_.size() > 1) {
        avg_fps_ = (avg_fps_ + fps_) / 2.0;
    }

    // Calculate frame time
    if (timestamps_.size() >= 2) {
        auto last_frame_duration = std::chrono::duration_cast<std::chrono::microseconds>(
            timestamps_.back() - timestamps_[timestamps_.size() - 2]);
        frame_time_ms_ = last_frame_duration.count() / 1000.0;
    }
}

void FPSCounter::reset() {
    timestamps_.clear();
    fps_ = 0.0;
    avg_fps_ = 0.0;
    frame_time_ms_ = 0.0;
}

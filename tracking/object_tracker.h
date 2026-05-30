#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <map>
#include "kalman_filter.h"

class ObjectTracker {
public:
    struct TrackedObject {
        int track_id;
        cv::Rect bbox;
        cv::Point2f center;
        std::shared_ptr<KalmanFilter> kf;
        int frame_count;
        int age;
    };

    ObjectTracker();
    ~ObjectTracker();

    // Update tracker with new detections
    void update(const std::vector<cv::Rect>& detections);

    // Get tracked objects
    std::vector<TrackedObject> getTrackedObjects() const { return tracked_objects_; }

    // Set parameters
    void setMaxDistance(float distance) { max_distance_ = distance; }
    void setMinFrames(int frames) { min_frames_ = frames; }

private:
    std::vector<TrackedObject> tracked_objects_;
    int next_track_id_ = 0;
    float max_distance_ = 50.0f;
    int min_frames_ = 3;

    // Helper methods
    float computeDistance(const cv::Rect& a, const cv::Rect& b);
    std::vector<std::pair<int, int>> associateDetections(const std::vector<cv::Rect>& detections);
};

#include "object_tracker.h"
#include <cmath>
#include <algorithm>

ObjectTracker::ObjectTracker() : next_track_id_(0) {}

ObjectTracker::~ObjectTracker() {}

void ObjectTracker::update(const std::vector<cv::Rect>& detections) {
    // Predict positions using Kalman filters
    for (auto& obj : tracked_objects_) {
        obj.kf->predict();
    }

    // Associate detections with tracked objects
    auto associations = associateDetections(detections);

    // Update tracked objects
    std::vector<bool> detection_used(detections.size(), false);
    for (const auto& [track_idx, det_idx] : associations) {
        if (track_idx >= 0 && track_idx < static_cast<int>(tracked_objects_.size())) {
            if (det_idx >= 0 && det_idx < static_cast<int>(detections.size())) {
                tracked_objects_[track_idx].bbox = detections[det_idx];
                tracked_objects_[track_idx].center = cv::Point2f(
                    detections[det_idx].x + detections[det_idx].width / 2.0f,
                    detections[det_idx].y + detections[det_idx].height / 2.0f
                );
                tracked_objects_[track_idx].frame_count++;
                detection_used[det_idx] = true;

                // Update Kalman filter
                cv::Mat measurement = (cv::Mat_<float>(2, 1) << tracked_objects_[track_idx].center.x,
                                                                tracked_objects_[track_idx].center.y);
                tracked_objects_[track_idx].kf->update(measurement);
            }
        }
    }

    // Create new tracks for unmatched detections
    for (size_t i = 0; i < detections.size(); ++i) {
        if (!detection_used[i]) {
            TrackedObject new_obj;
            new_obj.track_id = next_track_id_++;
            new_obj.bbox = detections[i];
            new_obj.center = cv::Point2f(
                detections[i].x + detections[i].width / 2.0f,
                detections[i].y + detections[i].height / 2.0f
            );
            new_obj.kf = std::make_shared<KalmanFilter>();
            new_obj.frame_count = 1;
            new_obj.age = 1;

            tracked_objects_.push_back(new_obj);
        }
    }

    // Remove dead tracks
    auto it = tracked_objects_.begin();
    while (it != tracked_objects_.end()) {
        if (it->frame_count > 30) {  // Dead track threshold
            it = tracked_objects_.erase(it);
        } else {
            ++it;
        }
    }
}

float ObjectTracker::computeDistance(const cv::Rect& a, const cv::Rect& b) {
    cv::Point2f center_a(a.x + a.width / 2.0f, a.y + a.height / 2.0f);
    cv::Point2f center_b(b.x + b.width / 2.0f, b.y + b.height / 2.0f);
    return cv::norm(center_a - center_b);
}

std::vector<std::pair<int, int>> ObjectTracker::associateDetections(const std::vector<cv::Rect>& detections) {
    std::vector<std::pair<int, int>> associations;

    for (size_t i = 0; i < tracked_objects_.size(); ++i) {
        float min_dist = max_distance_;
        int best_det = -1;

        for (size_t j = 0; j < detections.size(); ++j) {
            float dist = computeDistance(tracked_objects_[i].bbox, detections[j]);
            if (dist < min_dist) {
                min_dist = dist;
                best_det = j;
            }
        }

        associations.push_back({i, best_det});
    }

    return associations;
}

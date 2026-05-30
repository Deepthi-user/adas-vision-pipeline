#include "kalman_filter.h"

KalmanFilter::KalmanFilter() {
    // Initialize Kalman filter for 2D position tracking
    kf_.init(4, 2, 0);  // 4D state (x, y, vx, vy), 2D measurement (x, y)

    // Set transition matrix
    kf_.transitionMatrix = (cv::Mat_<float>(4, 4) <<
        1, 0, 1, 0,
        0, 1, 0, 1,
        0, 0, 1, 0,
        0, 0, 0, 1);

    // Set measurement matrix
    kf_.measurementMatrix = (cv::Mat_<float>(2, 4) <<
        1, 0, 0, 0,
        0, 1, 0, 0);

    // Set noise covariances
    cv::setIdentity(kf_.processNoiseCov, cv::Scalar::all(0.03));
    cv::setIdentity(kf_.measurementNoiseCov, cv::Scalar::all(0.1));
    cv::setIdentity(kf_.errorCovPost, cv::Scalar::all(1.0));

    state_ = cv::Mat::zeros(4, 1, CV_32F);
    measurement_ = cv::Mat::zeros(2, 1, CV_32F);
}

KalmanFilter::~KalmanFilter() {}

cv::Mat KalmanFilter::predict() {
    return kf_.predict();
}

void KalmanFilter::update(const cv::Mat& measurement) {
    measurement_ = measurement.clone();
    kf_.correct(measurement_);
}

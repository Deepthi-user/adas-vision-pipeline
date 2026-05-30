#include "calibrator.h"
#include <iostream>
#include <opencv2/calib3d.hpp>

Calibrator::Calibrator() {}

Calibrator::~Calibrator() {}

bool Calibrator::calibrateFromImages(const std::vector<std::string>& image_paths,
                                   cv::Size checkerboard_size,
                                   CalibrationResult& result) {
    std::vector<std::vector<cv::Point3f>> object_points;
    std::vector<std::vector<cv::Point2f>> image_points;

    // Create object points
    std::vector<cv::Point3f> obj_pts;
    for (int i = 0; i < checkerboard_size.height; ++i) {
        for (int j = 0; j < checkerboard_size.width; ++j) {
            obj_pts.push_back(cv::Point3f(j, i, 0.0f));
        }
    }

    // Process each image
    for (const auto& path : image_paths) {
        cv::Mat image = cv::imread(path);
        if (image.empty()) continue;

        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

        std::vector<cv::Point2f> corners;
        bool found = cv::findChessboardCorners(gray, checkerboard_size, corners);

        if (found) {
            cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
                           cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.1));
            object_points.push_back(obj_pts);
            image_points.push_back(corners);
        }
    }

    if (object_points.empty()) {
        std::cerr << "No checkerboard corners found" << std::endl;
        return false;
    }

    // Calibrate camera
    std::vector<cv::Mat> rvecs, tvecs;
    cv::calibrateCamera(object_points, image_points,
                       cv::Size(640, 480),  // Image size
                       result.camera_matrix,
                       result.dist_coeffs,
                       rvecs, tvecs);

    calib_result_ = result;
    is_calibrated_ = true;

    std::cout << "Camera calibrated successfully" << std::endl;
    return true;
}

cv::Mat Calibrator::undistortImage(const cv::Mat& image) {
    if (!is_calibrated_) {
        return image.clone();
    }

    cv::Mat undistorted;
    cv::undistort(image, undistorted, calib_result_.camera_matrix, calib_result_.dist_coeffs);
    return undistorted;
}

bool Calibrator::saveCalibration(const std::string& filepath) {
    if (!is_calibrated_) return false;

    cv::FileStorage fs(filepath, cv::FileStorage::WRITE);
    fs << "camera_matrix" << calib_result_.camera_matrix;
    fs << "dist_coeffs" << calib_result_.dist_coeffs;
    fs.release();

    return true;
}

bool Calibrator::loadCalibration(const std::string& filepath) {
    cv::FileStorage fs(filepath, cv::FileStorage::READ);
    if (!fs.isOpened()) return false;

    fs["camera_matrix"] >> calib_result_.camera_matrix;
    fs["dist_coeffs"] >> calib_result_.dist_coeffs;
    fs.release();

    is_calibrated_ = !calib_result_.camera_matrix.empty();
    return is_calibrated_;
}

#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

class Calibrator {
public:
    struct CalibrationResult {
        cv::Mat camera_matrix;
        cv::Mat dist_coeffs;
        double reprojection_error;
    };

    Calibrator();
    ~Calibrator();

    // Calibrate camera from checkerboard images
    bool calibrateFromImages(const std::vector<std::string>& image_paths,
                            cv::Size checkerboard_size,
                            CalibrationResult& result);

    // Undistort image using calibration
    cv::Mat undistortImage(const cv::Mat& image);

    // Save/Load calibration
    bool saveCalibration(const std::string& filepath);
    bool loadCalibration(const std::string& filepath);

private:
    CalibrationResult calib_result_;
    bool is_calibrated_ = false;
};

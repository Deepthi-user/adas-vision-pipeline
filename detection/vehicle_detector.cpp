#include "vehicle_detector.h"
#include <iostream>
#include <fstream>

VehicleDetector::VehicleDetector(const std::string& model_path) {
    // Load YOLO model (weights and config)
    try {
        // This is a placeholder - actual implementation would load YOLO weights
        std::cout << "Loading YOLO model from: " << model_path << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed to load model: " << e.what() << std::endl;
    }
}

VehicleDetector::~VehicleDetector() {}

bool VehicleDetector::detect(const cv::Mat& image, std::vector<Detection>& detections) {
    if (image.empty() || net_.empty()) {
        return false;
    }

    auto start = cv::getTickCount();

    // Prepare blob
    cv::Mat blob = cv::dnn::blobFromImage(image, 1.0 / 255.0,
                                         cv::Size(input_width_, input_height_),
                                         cv::Scalar(0, 0, 0), true, false);

    // Set input
    net_.setInput(blob);

    // Forward pass
    std::vector<cv::Mat> outs;
    std::vector<std::string> outNames = net_.getUnconnectedOutLayersNames();
    net_.forward(outs, outNames);

    // Post-process detections
    detections.clear();
    int frame_width = image.cols;
    int frame_height = image.rows;

    for (const auto& out : outs) {
        float* data = (float*)out.data;
        for (int j = 0; j < out.rows; ++j, data += out.cols) {
            cv::Mat scores = out.row(j).colRange(5, out.cols);
            cv::Point classIdPoint;
            double confidence;
            cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);

            if (confidence > confidence_threshold_) {
                int centerX = (int)(data[0] * frame_width);
                int centerY = (int)(data[1] * frame_height);
                int width = (int)(data[2] * frame_width);
                int height = (int)(data[3] * frame_height);

                Detection det;
                det.bbox = cv::Rect(centerX - width / 2, centerY - height / 2, width, height);
                det.confidence = static_cast<float>(confidence);
                det.class_id = classIdPoint.x;

                detections.push_back(det);
            }
        }
    }

    auto end = cv::getTickCount();
    inference_fps_ = cv::getTickFrequency() / (end - start);

    return true;
}

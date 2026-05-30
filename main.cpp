#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>

#include "camera/camera_driver.h"
#include "detection/lane_detector.h"
#include "detection/vehicle_detector.h"
#include "detection/sign_detector.h"
#include "tracking/object_tracker.h"
#include "visualization/visualizer.h"
#include "benchmarks/fps_counter.h"

struct PipelineConfig {
    std::string input_source;
    std::string output_file;
    bool is_video_file = false;
    int camera_index = 0;
    bool enable_visualization = true;
    bool enable_benchmarking = true;
    std::string calibration_file;
};

class ADASPipeline {
private:
    PipelineConfig config_;
    CameraDriver camera_;
    std::unique_ptr<LaneDetector> lane_detector_;
    std::unique_ptr<VehicleDetector> vehicle_detector_;
    std::unique_ptr<SignDetector> sign_detector_;
    ObjectTracker tracker_;
    std::unique_ptr<Visualizer> visualizer_;
    FPSCounter fps_counter_;
    bool is_running_ = false;

public:
    ADASPipeline(const PipelineConfig& config) : config_(config), fps_counter_(30) {}

    bool initialize() {
        // Initialize camera
        if (config_.is_video_file) {
            if (!camera_.initializeFromFile(config_.input_source)) {
                std::cerr << "Failed to initialize camera from file" << std::endl;
                return false;
            }
        } else {
            if (!camera_.initializeFromCamera(config_.camera_index)) {
                std::cerr << "Failed to initialize camera" << std::endl;
                return false;
            }
        }

        // Initialize detectors
        lane_detector_ = std::make_unique<LaneDetector>(camera_.getHeight(), camera_.getWidth());
        vehicle_detector_ = std::make_unique<VehicleDetector>("models/yolov3.weights");
        sign_detector_ = std::make_unique<SignDetector>("models/sign_detector.pt");

        // Initialize visualizer
        visualizer_ = std::make_unique<Visualizer>(camera_.getWidth(), camera_.getHeight());

        std::cout << "ADAS Pipeline initialized successfully" << std::endl;
        std::cout << "Resolution: " << camera_.getWidth() << "x" << camera_.getHeight() << std::endl;
        std::cout << "FPS: " << camera_.getFPS() << std::endl;

        return true;
    }

    void run() {
        is_running_ = true;
        std::cout << "Starting ADAS Vision Pipeline..." << std::endl;
        std::cout << "Press ESC to exit" << std::endl;

        CameraDriver::Frame frame;
        while (is_running_ && camera_.getFrame(frame)) {
            processFrame(frame);
            fps_counter_.tick();
        }

        printStatistics();
    }

    void stop() {
        is_running_ = false;
    }

private:
    void processFrame(const CameraDriver::Frame& frame) {
        cv::Mat output = frame.image.clone();

        // Lane detection
        LaneDetector::Lane lane;
        if (lane_detector_->detect(frame.image, lane)) {
            visualizer_->drawLanes(output, lane.left_points, lane.right_points);
        }

        // Vehicle detection
        std::vector<VehicleDetector::Detection> vehicles;
        if (vehicle_detector_->detect(frame.image, vehicles)) {
            std::vector<cv::Rect> bboxes;
            for (const auto& vehicle : vehicles) {
                visualizer_->drawBoundingBox(output, vehicle.bbox,
                                            vehicle.class_name + " (" +
                                            std::to_string(static_cast<int>(vehicle.confidence * 100)) + "%)");
                bboxes.push_back(vehicle.bbox);
            }
            // Update tracker
            tracker_.update(bboxes);
        }

        // Sign detection
        std::vector<SignDetector::Sign> signs;
        if (sign_detector_->detect(frame.image, signs)) {
            for (const auto& sign : signs) {
                visualizer_->drawBoundingBox(output, sign.bbox, sign.class_name,
                                            cv::Scalar(255, 0, 0));
            }
        }

        // Display FPS
        visualizer_->drawFPS(output, fps_counter_.getFPS());

        // Visualization
        if (config_.enable_visualization) {
            visualizer_->displayFrame(output);
        }

        // Write output if specified
        if (!config_.output_file.empty()) {
            static bool writer_initialized = false;
            if (!writer_initialized) {
                visualizer_->initializeVideoWriter(config_.output_file, camera_.getFPS());
                writer_initialized = true;
            }
            visualizer_->writeFrame(output);
        }
    }

    void printStatistics() {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "PIPELINE STATISTICS" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        std::cout << "Average FPS: " << std::fixed << std::setprecision(2)
                 << fps_counter_.getAverageFPS() << std::endl;
        std::cout << "Current FPS: " << fps_counter_.getFPS() << std::endl;
        std::cout << "Frame Time: " << std::fixed << std::setprecision(2)
                 << fps_counter_.getFrameTime() << " ms" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
    }
};

void printUsage(const char* program_name) {
    std::cout << "ADAS Vision Pipeline" << std::endl;
    std::cout << "Usage: " << program_name << " [options]" << std::endl;
    std::cout << "\nOptions:" << std::endl;
    std::cout << "  --video FILE          Input video file" << std::endl;
    std::cout << "  --camera INDEX        Camera index (default: 0)" << std::endl;
    std::cout << "  --output FILE         Output video file" << std::endl;
    std::cout << "  --calibration FILE    Camera calibration file" << std::endl;
    std::cout << "  --help                Show this help message" << std::endl;
}

int main(int argc, char* argv[]) {
    PipelineConfig config;
    config.input_source = "0";  // Default to webcam

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--video" && i + 1 < argc) {
            config.input_source = argv[++i];
            config.is_video_file = true;
        } else if (arg == "--camera" && i + 1 < argc) {
            config.camera_index = std::stoi(argv[++i]);
        } else if (arg == "--output" && i + 1 < argc) {
            config.output_file = argv[++i];
        } else if (arg == "--calibration" && i + 1 < argc) {
            config.calibration_file = argv[++i];
        } else if (arg == "--help") {
            printUsage(argv[0]);
            return 0;
        }
    }

    // Create and run pipeline
    ADASPipeline pipeline(config);

    if (!pipeline.initialize()) {
        std::cerr << "Failed to initialize pipeline" << std::endl;
        return 1;
    }

    pipeline.run();

    return 0;
}

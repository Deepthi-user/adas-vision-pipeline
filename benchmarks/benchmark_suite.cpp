#include "fps_counter.h"
#include <iostream>
#include <chrono>
#include <iomanip>

class BenchmarkSuite {
public:
    BenchmarkSuite();
    ~BenchmarkSuite();

    // Run benchmarks
    void runFullPipelineBenchmark(int num_frames);
    void printResults();

private:
    struct BenchmarkResult {
        std::string name;
        double min_fps = 0.0;
        double max_fps = 0.0;
        double avg_fps = 0.0;
        double total_time_ms = 0.0;
    };

    std::vector<BenchmarkResult> results_;
};

BenchmarkSuite::BenchmarkSuite() {}

BenchmarkSuite::~BenchmarkSuite() {}

void BenchmarkSuite::runFullPipelineBenchmark(int num_frames) {
    std::cout << "Running full pipeline benchmark for " << num_frames << " frames..." << std::endl;

    FPSCounter counter(30);
    BenchmarkResult result;
    result.name = "Full Pipeline";

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_frames; ++i) {
        // Simulate processing
        counter.tick();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    result.total_time_ms = duration.count();
    result.avg_fps = counter.getAverageFPS();

    results_.push_back(result);
}

void BenchmarkSuite::printResults() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "BENCHMARK RESULTS" << std::endl;
    std::cout << std::string(60, '=') << std::endl;

    for (const auto& result : results_) {
        std::cout << std::left << std::setw(20) << result.name
                 << "Avg FPS: " << std::fixed << std::setprecision(2) << result.avg_fps
                 << " | Time: " << result.total_time_ms << " ms" << std::endl;
    }

    std::cout << std::string(60, '=') << std::endl;
}

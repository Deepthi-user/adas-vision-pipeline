# ADAS Vision Pipeline - Setup Guide

## System Requirements

### Minimum
- CPU: Intel i5 or equivalent
- RAM: 4GB
- OS: Linux, macOS, or Windows

### Recommended
- CPU: Intel i7 or Ryzen 7
- GPU: NVIDIA GPU with CUDA support
- RAM: 8GB+
- OS: Ubuntu 20.04 LTS

## Dependencies

### Core Libraries
- **OpenCV** 4.5.0+
- **CMake** 3.16+
- **C++17** compatible compiler

### Optional
- **CUDA** 11.0+ (for GPU acceleration)
- **cuDNN** (for deep learning optimization)
- **PyTorch/ONNX** (for model inference)

## Installation

### Ubuntu/Debian

```bash
# Update package manager
sudo apt-get update
sudo apt-get upgrade -y

# Install dependencies
sudo apt-get install -y \
    build-essential \
    cmake \
    git \
    libopencv-dev \
    libpthread-stubs0-dev \
    pkg-config

# Optional: Install CUDA
wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/x86_64/cuda-repo-ubuntu2004_11.3.0-1_amd64.deb
sudo dpkg -i cuda-repo-ubuntu2004_11.3.0-1_amd64.deb
sudo apt-key adv --fetch-keys https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/x86_64/3bf863cc.pub
sudo apt-get update
sudo apt-get install -y cuda-11-3
```

### macOS

```bash
# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake opencv
```

### Build from Source

```bash
# Clone repository
git clone https://github.com/yourusername/adas-vision-pipeline.git
cd adas-vision-pipeline

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
make -j$(nproc)

# Optional: Install
sudo make install
```

## Configuration

### Camera Calibration

1. Capture checkerboard images:
```bash
./adas_pipeline --calibrate --camera 0 --output calibration_images/
```

2. Run calibration:
```bash
./adas_pipeline --run-calibration calibration_images/ --output calib.yaml
```

### Model Download

Download YOLO weights:
```bash
wget https://pjreddie.com/media/files/yolov3.weights
wget https://raw.githubusercontent.com/pjreddie/darknet/master/cfg/yolov3.cfg
wget https://raw.githubusercontent.com/pjreddie/darknet/master/data/coco.names

mkdir models
mv yolov3* models/
```

## Running

### From Video File
```bash
./adas_pipeline --video input.mp4 --output output.mp4
```

### From Webcam
```bash
./adas_pipeline --camera 0
```

### With Calibration
```bash
./adas_pipeline --camera 0 --calibration calib.yaml
```

### Benchmarking
```bash
./adas_pipeline --benchmark --frames 1000
```

## Troubleshooting

### OpenCV Not Found
```bash
export OpenCV_DIR=/path/to/opencv/install
cmake ..
```

### CUDA Errors
```bash
# Check CUDA installation
nvcc --version

# Add to PATH
export PATH=/usr/local/cuda/bin:$PATH
export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH
```

### Performance Issues
1. Check FPS counter output
2. Profile with bottleneck identification
3. Reduce resolution or frame skipping
4. Enable GPU acceleration if available

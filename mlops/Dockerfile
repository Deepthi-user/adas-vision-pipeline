FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV PYTHONUNBUFFERED=1

RUN apt-get update && apt-get install -y \
    build-essential cmake git wget python3 python3-pip \
    libopencv-dev libpthread-stubs0-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
COPY . .

RUN pip install --no-cache-dir -r mlops/requirements.txt && \
    mkdir build && cd build && \
    cmake .. && make -j$(nproc)

EXPOSE 8000
CMD ["./build/adas_pipeline", "--camera", "0"]

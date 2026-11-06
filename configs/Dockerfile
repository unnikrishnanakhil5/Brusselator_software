# C++ Lecture Environment
# Usage:
#   Build:  docker build -t brusselator-image .
#   Run:   docker run -it --hostname mycdfsolver-container -p 8888:8888 -v $(pwd):/workspace brusselator-image

# Base on minimal Debian image
FROM debian:stable-slim
 
# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive
 
# install tools
RUN apt-get update && apt-get install -y \
    gcc \
    g++ \
    make \
    cmake \
    gdb \
    git \
    neovim \
    libboost-all-dev \
    python3.13-venv \
    tree \
    paraview \
    libxcb-cursor0 \
    libgtest-dev \
    libeigen3-dev \
    libvtk9-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*
 
# Set working directory
WORKDIR /workspace

# add custom configs
COPY custom_bashrc /root/.bashrc
COPY neovim_config /root/.config/nvim/init.lua
COPY jupyter_lab_config.py /root/.jupyter/

# Open bash shell
CMD ["/bin/bash"]


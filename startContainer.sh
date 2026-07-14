#!/bin/bash

docker run -it --hostname mycfdsolver-container -p 8888:8888 -v $(pwd):/workspace -e DISPLAY=$DISPLAY -e WAYLAND_DISPLAY=$WAYLAND_DISPLAY -e XDG_RUNTIME_DIR=/mnt/wslg/runtime-dir -v /mnt/wslg/runtime-dir:/mnt/wslg/runtime-dir -v /tmp/.X11-unix:/tmp/.X11-unix brusselator-image

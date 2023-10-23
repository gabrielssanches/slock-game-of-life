#!/bin/bash

docker run \
    -v `pwd`:/game \
    -it \
    raylib/linux_x64 \
    /bin/bash -c "source /usr/local/bin/entrypoint.sh && make V=1"

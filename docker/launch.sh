#!/bin/bash

docker run \
    -v `pwd`:/game \
    -ti \
    raylib/linux_x64 \
    /bin/bash -c "source /usr/local/bin/entrypoint.sh && /bin/bash"

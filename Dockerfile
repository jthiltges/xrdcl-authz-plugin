FROM opensciencegrid/software-base:3.6-el8-release as builder

RUN    yum install -y \
           cmake \
           git \
           gcc-c++ \
           xrootd-client-devel \
    && rm -rf /var/cache/

WORKDIR /work
COPY . /work

RUN    mkdir build \
    && cd build \
    && cmake .. -DCMAKE_INSTALL_PREFIX=/ -DLIB_INSTALL_DIR=/usr/lib64 \
    && make install

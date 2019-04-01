FROM ubuntu
MAINTAINER k-ishigaki <k-ishigaki@frontier.hokudai.ac.jp>

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libncurses-dev \
    zlib1g-dev \
    wget \
    && apt-get clean

RUN git clone --depth=1 --recursive https://github.com/MaskRay/ccls \
    && cd ccls \
    && wget -c http://releases.llvm.org/7.0.1/clang+llvm-7.0.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz \
    && tar xf clang+llvm-7.0.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz \
    && cmake -H. -BRelease -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=$PWD/clang+llvm-7.0.1-x86_64-linux-gnu-ubuntu-16.04 \
    && cmake --build Release

ENV PATH $PATH:/ccls/Release

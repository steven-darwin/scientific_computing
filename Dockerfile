FROM ghcr.io/steven-darwin/scientific_computing/toolchain:latest as preparation

WORKDIR /home
RUN mkdir scientific_computing
WORKDIR ./scientific_computing
RUN mkdir shared_lib

FROM scratch as dependency

# Resolve vcpkg package
WORKDIR /home/scientific_computing/shared_lib
COPY ./shared_lib/vcpkg.json ./shared_lib/vcpkg-configuration.json ./
RUN vcpkg install

# Resolve XDMF library
WORKDIR /home/external
RUN git clone https://gitlab.kitware.com/xdmf/xdmf.git
WORKDIR ./xdmf
RUN git checkout 04a84bab0eb1568e0f1a27c8fb60c6931efda003

WORKDIR /home/scientific_computing/shared_lib
RUN mkdir xdmf-3.0.0

FROM scratch as final

WORKDIR /home/scientific_computing
COPY ./CMakeLists.txt ./CMakePresets.json ./CMakeUserPresets.json ./
RUN mkdir include
COPY ./include ./include
RUN mkdir src
COPY ./src .src
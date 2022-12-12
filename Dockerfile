# mmCEsim Dockerfile
# https://mmcesim.org

FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y build-essential cmake libboost-all-dev
WORKDIR /app/bin
COPY CMakeLists.txt ..
COPY src ../src
COPY include ../include
COPY test ../test
COPY ext ../ext
RUN cmake ..
CMD ["make"]

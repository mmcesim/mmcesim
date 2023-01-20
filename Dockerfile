# mmCEsim Dockerfile
# https://mmcesim.org

FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y build-essential cmake libboost-all-dev
WORKDIR /mmcesim/build
RUN mkdir ../doc
COPY CMakeLists.txt ..
COPY LICENSE ..
COPY README.md ..
COPY CONTRIBUTING.md ..
COPY CODE_OF_CONDUCT.md ..
COPY doc/mmCEsim-doc.pdf ../doc
COPY src ../src
COPY include ../include
COPY test ../test
COPY tools ../tools
COPY ext ../ext
RUN cmake ..
RUN make
RUN make install
ENTRYPOINT [ "bash" ]

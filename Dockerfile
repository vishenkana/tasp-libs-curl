FROM tasp/libcommon:1.0.2

COPY . /curl
WORKDIR /curl

RUN export DEBIAN_FRONTEND=noninteractive && \
    apt-get update && apt-get install -y --no-install-recommends --reinstall \
        libcurl4-openssl-dev

RUN mkdir build && cd build && cmake .. && ninja install

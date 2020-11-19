FROM ubuntu:focal-20201008
LABEL maintainer="HyodaKazuaki"

WORKDIR /app

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        git \
        gdb \
        build-essential \
        flex \
        bison && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

COPY ./ ./

CMD ["/bin/sh", "-c", "while :; do sleep 10; done"]
FROM ubuntu:latest
Maintainer "Sanaf <sanaf@0x30c4.dev>"

RUN apt -y update && \
    apt install -y gcc make git && \
    mkdir /app && cd / && \
    git clone https://github.com/0x30c4/Basic-Web-Server-And-LoadBalancer-With-C.git app

WORKDIR /app

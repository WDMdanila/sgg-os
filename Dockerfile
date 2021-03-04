FROM debian:stretch

RUN apt update
RUN apt install -y g++ binutils libc6-dev-i386 make grub-common grub-pc-bin xorriso

VOLUME /build_env
WORKDIR /build_env
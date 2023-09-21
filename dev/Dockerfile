FROM ubuntu AS base
USER root
RUN apt-get update && \
  apt-get upgrade -y 
# essential tools
RUN apt-get install -y git vim gcc clang binutils-i586-linux-gnu valgrind gdb make
# libs
RUN apt-get install -y libssl-dev libwebsockets-dev

RUN apt-get clean autoclean && \
  apt-get autoremove --yes

# FROM base AS shammas
# ARG TAGS
# settings shammas as password
RUN useradd -m shammas && \
  echo "shammas:shammas" | chpasswd && \
  adduser shammas sudo
USER shammas
WORKDIR /home/shammas

# FROM shammas
# COPY . .
# CMD sh -c "ansible-playbook $TAGS local.yml --ask-become-pass; zsh"

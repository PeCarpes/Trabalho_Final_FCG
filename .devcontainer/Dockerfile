FROM mcr.microsoft.com/devcontainers/cpp:1-ubuntu-24.04

ARG REINSTALL_CMAKE_VERSION_FROM_SOURCE="3.31.0"

COPY ./reinstall-cmake.sh /tmp/

ENV DEBIAN_FRONTEND=noninteractive

RUN if [ "${REINSTALL_CMAKE_VERSION_FROM_SOURCE}" != "none" ]; then \
        chmod +x /tmp/reinstall-cmake.sh && /tmp/reinstall-cmake.sh ${REINSTALL_CMAKE_VERSION_FROM_SOURCE}; \
    fi && rm -f /tmp/reinstall-cmake.sh

# Atualiza sistema e instala dependências
RUN apt-get update -y && apt-get upgrade -y && \
    apt-get install -y --no-install-recommends \
    build-essential make libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxcb1-dev libxext-dev \
    libxrender-dev libxfixes-dev libxau-dev libxdmcp-dev libxxf86vm-dev libglm-dev && \
    rm -rf /var/lib/apt/lists/*

# Instala dependências adicionais para o OpenGL
RUN apt-get update && apt-get install -y --no-install-recommends \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*

# Instala dependências para o OpenGL no Windows
RUN apt-get update && apt-get install -y \
    cmake \
    make \
    g++ \
    mingw-w64


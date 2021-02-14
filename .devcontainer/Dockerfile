#-------------------------------------------------------------------------------------------------------------
# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License. See https://go.microsoft.com/fwlink/?linkid=2090316 for license information.
#-------------------------------------------------------------------------------------------------------------
FROM debian:10

# Avoid warnings by switching to noninteractive
ENV DEBIAN_FRONTEND=noninteractive

ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID
# Configure apt and install packages
RUN apt-get update \
    && apt-get -y install --no-install-recommends apt-utils dialog software-properties-common 2>&1 \
    # 
    # Verify git, process tools, lsb-release (useful for CLI installs) installed
    && apt-get -y install git iproute2 procps lsb-release wget gnupg2 \
    #
    # Install C++ tools
    && apt-get -y install build-essential cmake cppcheck valgrind
    #
    # Install current clang compiler
    RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key| apt-key add \
    # Fingerprint: 6084 F3CF 814B 57C1 CF12 EFD5 15CF 4D18 AF4F 7421 \
    && add-apt-repository -y 'deb http://apt.llvm.org/buster/ llvm-toolchain-buster main' \
    && apt-get update \
    && apt-get -y install clang-format clang-tidy clang clangd \
      libc++-dev libc++1 libc++abi-dev libc++abi1 libclang-dev libclang1 \
      liblldb-dev libllvm-ocaml-dev libomp-dev libomp5 lld lldb \
      llvm-dev llvm-runtime llvm python-clang \
    #
    # Install Python tools
    && apt-get -y install python3-venv python3-pip \
    #
    # Create a non-root user to use if preferred - see https://aka.ms/vscode-remote/containers/non-root-user.
    && groupadd --gid $USER_GID $USERNAME \
    && useradd -s /bin/bash --uid $USER_UID --gid $USER_GID -m $USERNAME \
    # [Optional] Add sudo support for the non-root user
    && apt-get install -y sudo \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME\
    && chmod 0440 /etc/sudoers.d/$USERNAME \
    #
    # Clean up
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*

# Install conan
RUN pip3 install conan

# Switch back to dialog for any ad-hoc use of apt-get
ENV DEBIAN_FRONTEND=

USER $USERNAME
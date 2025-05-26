# syntax=docker/dockerfile:1.14.0-labs

ARG TARGETARCH
ARG DEB_VERSION="bookworm"

# Base Image
FROM mcr.microsoft.com/devcontainers/base:${DEB_VERSION}

# +-----------------------------+
# | REUSE GLOBAL ARGS           |
# +-----------------------------+

ARG TARGETARCH
ARG DEB_VERSION
ARG LLVM_VERSION

# +-----------------------------+
# | PRE-REQUISITE/INIT PACKAGES |
# +-----------------------------+

# Install dependencies
RUN --mount=type=cache,id=apt-lib,target=/var/lib/apt,sharing=locked \
    --mount=type=cache,id=debconf,target=/var/cache/debconf,sharing=locked \
    apt-get update

RUN --mount=type=cache,id=apt-cache,target=/var/cache/apt/archives,sharing=locked \
    --mount=type=cache,id=apt-lib,target=/var/lib/apt,sharing=locked \
    --mount=type=cache,id=debconf,target=/var/cache/debconf,sharing=locked \
    apt-get install -y --no-install-recommends \
    locales  \
    && localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8

ENV LANG="en_US.utf8"
ENV DEBIAN_FRONTEND="noninteractive"

WORKDIR /tmp

RUN --mount=type=cache,id=apt-lib,target=/var/lib/apt,sharing=locked \
    --mount=type=cache,id=debconf,target=/var/cache/debconf,sharing=locked \
    apt-get update

# Install dependencies
RUN --mount=type=cache,id=apt-cache,target=/var/cache/apt/archives,sharing=locked \
    --mount=type=cache,id=apt-lib,target=/var/lib/apt,sharing=locked \
    --mount=type=cache,id=debconf,target=/var/cache/debconf,sharing=locked \
    apt-get install -y --no-install-recommends \
    gnupg2 \
    ca-certificates \
    software-properties-common \
    cppcheck \
    valgrind \
    ccache \
    cmake \
    git \
    nano \
    jq \
    # required by IDE
    gdb \
    make \
    ninja-build \
    rsync \
    zip \
    sudo \
    # required by VCPKG
    openssh-server \
    tar \
    curl \
    unzip \
    pkg-config \
    bash-completion \
    aria2 \
    mono-complete

# # +-----------------------------+
# # | CMake                       |
# # +-----------------------------+

ARG CMAKE_ARCH=${TARGETARCH}
ARG CMAKE_ARCH=${CMAKE_ARCH/arm64/'linux-aarch64'}
ARG CMAKE_ARCH=${CMAKE_ARCH/amd64/'linux-x86_64'}

RUN version="$(curl -s https://api.github.com/repos/Kitware/CMake/releases/latest | jq -r '.tag_name' | cut -c 2-)"; echo "Latest Version: ${version}" \
    && url="https://github.com/Kitware/CMake/releases/download/v${version}/cmake-${version}-${CMAKE_ARCH}.sh"; echo "Download URL: ${url}" \
    && curl -fsSL "${url}" -o "cmake-${version}-${CMAKE_ARCH}.sh" \
    && mkdir /opt/cmake \
    && sh "cmake-${version}-${CMAKE_ARCH}.sh" --prefix=/opt/cmake --skip-license \
    && ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake \
    && rm -rf "cmake-${version}-${CMAKE_ARCH}.sh"

# +-----------------------------+
# | LLVM                        |
# +-----------------------------+

ARG LLVM_VERSION="19"

RUN --mount=type=cache,id=apt-lib,target=/var/lib/apt,sharing=locked \
    --mount=type=cache,id=debconf,target=/var/cache/debconf,sharing=locked \
    apt-get update

# Add LLVM repository
RUN --mount=type=cache,id=apt-cache,target=/var/cache/apt/archives,sharing=locked \
    --mount=type=cache,id=apt-lib,target=/var/lib/apt,sharing=locked \
    --mount=type=cache,id=debconf,target=/var/cache/debconf,sharing=locked \
    echo "deb http://apt.llvm.org/${DEB_VERSION}/ llvm-toolchain-bookworm-${LLVM_VERSION} main" > /etc/apt/sources.list.d/llvm.list \
    && echo "deb-src http://apt.llvm.org/${DEB_VERSION}/ llvm-toolchain-bookworm-${LLVM_VERSION} main" >> /etc/apt/sources.list.d/llvm.list \
    && curl -sSL https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
    && apt-get install -y --no-install-recommends \
    # LLVM \
    libllvm-19-ocaml-dev \
    libllvm19 \
    llvm-19 \
    llvm-19-dev \
    llvm-19-doc \
    llvm-19-examples \
    llvm-19-runtime \
    # Clang and co \
    clang-19 \
    clang-tools-19 \
    clang-19-doc \
    libclang-common-19-dev \
    libclang-19-dev \
    libclang1-19 \
    clang-format-19 \
    python3-clang-19 \
    clangd-19 \
    clang-tidy-19 \
    # compiler-rt \
    libclang-rt-19-dev \
    # polly \
    libpolly-19-dev \
    # libfuzzer \
    libfuzzer-19-dev \
    # lldb \
    lldb-19 \
    # lld (linker) \
    lld-19 \
    # libc++ \
    libc++-19-dev \
    libc++abi-19-dev \
    # OpenMP \
    libomp-19-dev \
    # libclc \
    libclc-19-dev \
    # libunwind \
    libunwind-19-dev \
    # mlir \
    libmlir-19-dev \
    mlir-19-tools \
    # bolt \
    libbolt-19-dev \
    bolt-19 \
    # flang \
    flang-19 \
    # wasm support \
    libclang-rt-19-dev-wasm32 \
    libclang-rt-19-dev-wasm64 \
    libc++-19-dev-wasm32 \
    libc++abi-19-dev-wasm32

# +-----------------------------+
# | OpenGL                      |
# +-----------------------------+

RUN --mount=type=cache,id=apt-cache,target=/var/cache/apt/archives,sharing=locked \
    --mount=type=cache,id=apt-lib,target=/var/lib/apt,sharing=locked \
    --mount=type=cache,id=debconf,target=/var/cache/debconf,sharing=locked \
    apt-get install -y --no-install-recommends \
    libxinerama-dev \
    libxcursor-dev \
    xorg-dev \
    libglu1-mesa-dev

# +-----------------------------+
# | ZSH                         |
# +-----------------------------+
# setup zsh, omyzsh, powerline fonts
# setup zsh plugins: autosuggestions, autocompletions, history search
# setup starship

ENV TERM=xterm-256color
ENV ZSH_THEME=agnoster

RUN --mount=type=cache,id=apt-lib,target=/var/lib/apt,sharing=locked \
    --mount=type=cache,id=debconf,target=/var/cache/debconf,sharing=locked \
    apt-get update

RUN --mount=type=cache,id=apt-cache,target=/var/cache/apt/archives,sharing=locked \
    --mount=type=cache,id=apt-lib,target=/var/lib/apt,sharing=locked \
    --mount=type=cache,id=debconf,target=/var/cache/debconf,sharing=locked \
    rm -rf ~/.oh-my-zsh/ \
    && apt-get install -y --no-install-recommends \
    zsh \
    powerline \
    fonts-powerline \
    && export ZSH_CUSTOM=${ZSH_CUSTOM:-~/.oh-my-zsh/custom} \
    && curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh | sh \
    && git clone --depth=1 https://github.com/zsh-users/zsh-autosuggestions \
    "${ZSH_CUSTOM}/plugins/zsh-autosuggestions" \
    && git clone --depth=1 https://github.com/zsh-users/zsh-completions \
    "${ZSH_CUSTOM}/plugins/zsh-completions" \
    && git clone --depth=1 https://github.com/zsh-users/zsh-history-substring-search \
    "${ZSH_CUSTOM}/plugins/zsh-history-substring-search" \
    && sed -i "s/plugins=(git)/plugins=(git zsh-completions zsh-autosuggestions zsh-history-substring-search)/" /root/.zshrc \
    && sed -i "s/ZSH_THEME=\"agnoster\"/ZSH_THEME=\"robbyrussell\"/" /root/.zshrc


ENV SHELL="/bin/zsh"

RUN sh -c "$(curl -fsSL https://starship.rs/install.sh)" -- --yes && \
    echo 'eval "$(starship init bash)"' >> ~/.bashrc && \
    echo 'eval "$(starship init zsh)"' >> /root/.zshrc

# +-----------------------------+
# | vcpkg                       |
# +-----------------------------+

ENV VCPKG_ROOT=/opt/vcpkg
ENV VCPKG_INSTALLATION_ROOT=/opt/vcpkg
ENV VCPKG_TRIPLET=x64-linux
ENV VCPKG_FORCE_SYSTEM_BINARIES=1
ENV VCPKG_USE_SYSTEM_BINARIES=1
ENV PATH="${PATH}:${VCPKG_ROOT}"

RUN git clone --depth=1 \
    -c core.eol=lf \
    -c core.autocrlf=false \
    -c fsck.zeroPaddedFilemode=ignore \
    -c fetch.fsck.zeroPaddedFilemode=ignore \
    -c receive.fsck.zeroPaddedFilemode=ignore \
    https://github.com/microsoft/vcpkg "${VCPKG_ROOT}" \
    && "${VCPKG_ROOT}"/bootstrap-vcpkg.sh -disableMetrics \
    && "${VCPKG_ROOT}"/vcpkg integrate install

# # +-----------------------------+
# # | CLEANUP                     |
# # +-----------------------------+

RUN apt-get autoremove -y \
    && apt-get clean autoclean \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /tmp/*

# # +-----------------------------+
# # | Set Locale + Timezone       |
# # +-----------------------------+

ENV TZ=Europe/Amsterdam

# configure german locale
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && \
    echo $TZ > /etc/timezone && \
    dpkg-reconfigure tzdata

ENV LANG=en_US.UTF-8 LC_ALL=en_US.UTF-8

WORKDIR /app
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

# +-----------------------------+
# | PRE-REQUISITE/INIT PACKAGES |
# +-----------------------------+

# Install dependencies
RUN \
    apt-get update

RUN \
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
RUN \
    apt-get install -y --no-install-recommends \
    gnupg2 \
    ca-certificates \
    software-properties-common \
    cppcheck \
    ccache \
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
    xxhash

# +-----------------------------+
# | LLVM                        |
# +-----------------------------+

ARG LLVM_VERSION="19"

# Add LLVM repository
RUN \
    echo "deb http://apt.llvm.org/${DEB_VERSION}/ llvm-toolchain-bookworm-${LLVM_VERSION} main" > /etc/apt/sources.list.d/llvm.list \
    && echo "deb-src http://apt.llvm.org/${DEB_VERSION}/ llvm-toolchain-bookworm-${LLVM_VERSION} main" >> /etc/apt/sources.list.d/llvm.list \
    && curl -sSL https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
    # LLVM \
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
    # lldb \
    lldb-19 \
    # lld (linker) \
    lld-19 \
    # libc++ \
    libc++-19-dev \
    libc++abi-19-dev \
    # libclc \
    libclc-19-dev \
    # bolt \
    libbolt-19-dev \
    bolt-19 \
    # wasm support \
    libclang-rt-19-dev-wasm32 \
    libclang-rt-19-dev-wasm64 \
    libc++-19-dev-wasm32 \
    libc++abi-19-dev-wasm32

# +-----------------------------+
# | OpenGL                      |
# +-----------------------------+

RUN \
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

RUN \
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

ARG VCPKG_ROOT="/opt/vcpkg"
ENV PATH="${PATH}:${VCPKG_ROOT}"

ARG VCPKG_TRIPLET=${TARGETARCH}
ARG VCPKG_TRIPLET=${VCPKG_TRIPLET/arm64/'arm64-linux'}
ARG VCPKG_TRIPLET=${VCPKG_TRIPLET/amd64/'x64-linux'}
ENV VCPKG_TRIPLET="${VCPKG_TRIPLET}"

ENV VCPKG_ROOT="${VCPKG_ROOT}"
ENV VCPKG_INSTALLATION_ROOT="${VCPKG_ROOT}"

# TODO: check for crossplatform compilation
ENV VCPKG_FORCE_SYSTEM_BINARIES=1
ENV VCPKG_USE_SYSTEM_BINARIES=1

RUN git clone \
    -c core.eol=lf \
    -c core.autocrlf=false \
    -c fsck.zeroPaddedFilemode=ignore \
    -c fetch.fsck.zeroPaddedFilemode=ignore \
    -c receive.fsck.zeroPaddedFilemode=ignore \
    https://github.com/microsoft/vcpkg "${VCPKG_ROOT}" \
    && "${VCPKG_ROOT}"/bootstrap-vcpkg.sh -disableMetrics \
    && "${VCPKG_ROOT}"/vcpkg integrate install


# # +-----------------------------+
# # | CMake                       |
# # +-----------------------------+

ARG CMAKE_VERSION="4.0.2"
ARG CMAKE_ROOT="/opt/cmake"

ARG CMAKE_ARCH=${TARGETARCH}
ARG CMAKE_ARCH=${CMAKE_ARCH/arm64/'linux-aarch64'}
ARG CMAKE_ARCH=${CMAKE_ARCH/amd64/'linux-x86_64'}

ARG CMAKE_CHECKSUM=${TARGETARCH}
ARG CMAKE_CHECKSUM=${CMAKE_CHECKSUM/arm64/'b83ce5279133fd8863c6bc2f0afec4eb'}
ARG CMAKE_CHECKSUM=${CMAKE_CHECKSUM/amd64/''}

RUN --mount=type=cache,id=cmake-${CMAKE_VERSION}-${CMAKE_ARCH},target=/cache${CMAKE_ROOT},sharing=locked \
    [ "${CMAKE_CHECKSUM}" = "$(find /cache${CMAKE_ROOT} -type f -exec xxhsum -H128 {} + | sort | xxhsum -H128 | awk '{print $1}')" ] \
    && { \
    echo 'Move cached version to destination' \
    && mkdir -p "${CMAKE_ROOT}" \
    && echo 'Copy new version from cache' \
    && cp -r "/cache${CMAKE_ROOT}"/* "${CMAKE_ROOT}" \
    && ln -sf "${CMAKE_ROOT}/bin/ctest" '/usr/local/bin/ctest' \
    && ln -sf "${CMAKE_ROOT}/bin/cmake" '/usr/local/bin/cmake'; \
    } \
    || { \
    echo 'Clean cache folder' \
    && rm -rf "/cache${CMAKE_ROOT}"/* \
    && echo 'Download new version' \
    && curl -fsSL "https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-${CMAKE_ARCH}.sh" \
    -o "cmake.sh" \
    && echo 'Install new version' \
    && mkdir -p "${CMAKE_ROOT}" \
    && sh "cmake.sh" --prefix="${CMAKE_ROOT}" --skip-license \
    && ln -sf "${CMAKE_ROOT}/bin/ctest" '/usr/local/bin/ctest' \
    && ln -sf "${CMAKE_ROOT}/bin/cmake" '/usr/local/bin/cmake' \
    && rm -rf "cmake.sh" \
    && echo 'Copy new version to cache' \
    && cp -r "${CMAKE_ROOT}"/* "/cache${CMAKE_ROOT}" \
    && find "/cache${CMAKE_ROOT}" -type f -exec xxhsum -H128 {} + | sort | xxhsum -H128 | awk '{print $1}'; \
    }

# +-----------------------------+
# | LLVM-MinGW                  |
# +-----------------------------+

ARG LLVM_MINGV_VERSION="20250528"
ARG MINGW_ROOT="/opt/llvm-mingw"

ARG MINGW_ARCH=${TARGETARCH}
ARG MINGW_ARCH=${MINGW_ARCH/arm64/'aarch64'}
ARG MINGW_ARCH=${MINGW_ARCH/amd64/'x86_64'}

ARG MINGW_CHECKSUM=${TARGETARCH}
ARG MINGW_CHECKSUM=${MINGW_CHECKSUM/arm64/'56e757737e51fade97e1b2b025b2c1e3'}
ARG MINGW_CHECKSUM=${MINGW_CHECKSUM/amd64/''}


# https://github.com/mstorsjo/llvm-mingw/releases/download/20250514/llvm-mingw-20250514-ucrt-ubuntu-22.04-x86_64.tar.xz
RUN --mount=type=cache,id=llvm-mingw-${LLVM_MINGV_VERSION}-${MINGW_ARCH},target=/cache${MINGW_ROOT},sharing=locked \
    [ "${MINGW_CHECKSUM}" = "$(find /cache${MINGW_ROOT} -type f -exec xxhsum -H128 {} + | sort | xxhsum -H128 | awk '{print $1}')" ] \
    && { \
    echo 'Move cached version to destination' \
    && mkdir -p "${MINGW_ROOT}" \
    && cp -r "/cache${MINGW_ROOT}/llvm-mingw-${LLVM_MINGV_VERSION}-ucrt-ubuntu-22.04-${MINGW_ARCH}"/* "${MINGW_ROOT}"; \
    } \
    || { \
    echo 'Clean cache folder' \
    && rm -rf "/cache${MINGW_ROOT}"/* \
    && echo 'Download new version' \
    && curl -fsSL "https://github.com/mstorsjo/llvm-mingw/releases/download/${LLVM_MINGV_VERSION}/llvm-mingw-${LLVM_MINGV_VERSION}-ucrt-ubuntu-22.04-${MINGW_ARCH}.tar.xz" \
    -o "llvm-mingw.tar.xz" \
    && echo 'Extract new version to cache' \
    && tar -xf "llvm-mingw.tar.xz" -C "/cache${MINGW_ROOT}" \
    && rm -rf "llvm-mingw.tar.xz" \
    && find "/cache${MINGW_ROOT}/llvm-mingw-${LLVM_MINGV_VERSION}-ucrt-ubuntu-22.04-${MINGW_ARCH}" -type f -exec xxhsum -H128 {} + | sort | xxhsum -H128 | awk '{print $1}' \
    && echo 'Move new version to destination' \
    && mkdir -p "${MINGW_ROOT}" \
    && cp -r "/cache${MINGW_ROOT}/llvm-mingw-${LLVM_MINGV_VERSION}-ucrt-ubuntu-22.04-${MINGW_ARCH}"/* "${MINGW_ROOT}"; \
    }

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
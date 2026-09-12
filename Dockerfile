FROM ubuntu:24.04 AS builder


RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*


WORKDIR /opt


RUN git clone https://github.com/microsoft/vcpkg.git


RUN /opt/vcpkg/bootstrap-vcpkg.sh -disableMetrics


RUN /opt/vcpkg/vcpkg install \
    crow:x64-linux \
    sqlite3:x64-linux


WORKDIR /app


COPY CMakeLists.txt .

COPY src ./src


RUN cmake \
    -S . \
    -B build \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake


RUN cmake --build build -j2



FROM ubuntu:24.04 AS runtime


RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    libsqlite3-0 \
    && rm -rf /var/lib/apt/lists/*


WORKDIR /app


COPY --from=builder /app/build/UrlShortener /app/UrlShortener

COPY web /app/web


RUN mkdir -p /app/data


EXPOSE 8080


CMD ["./UrlShortener"]
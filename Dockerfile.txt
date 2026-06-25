# Этап сборки
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    libopencv-dev \
    libgtest-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN mkdir -p build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j"$(nproc)"

# Этап выполнения (минимальный образ)
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Устанавливаем полную рантайм-библиотеку OpenCV (имена под-пакетов из примера
# конфликтуют между версиями Ubuntu, поэтому используем общий пакет)
RUN apt-get update && apt-get install -y --no-install-recommends \
    libopencv-core4.5d \
    libopencv-imgproc4.5d \
    libopencv-imgcodecs4.5d \
    && rm -rf /var/lib/apt/lists/* \
    || (apt-get update && apt-get install -y --no-install-recommends libopencv-dev && rm -rf /var/lib/apt/lists/*)

WORKDIR /app
COPY --from=builder /app/build/image_editor /app/image_editor

# Создаём точки монтирования
VOLUME ["/input", "/output"]

ENTRYPOINT ["/app/image_editor"]
CMD ["--input", "/input", "--output", "/output"]

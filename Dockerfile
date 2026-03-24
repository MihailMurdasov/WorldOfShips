FROM ubuntu:24.04 AS builder

LABEL org.opencontainers.image.source="https://github.com/chesluchilos/WorldOfShips"

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ \
    make \
    libssl-dev \
    && rm -rf /var/lib/apt/lists/*

ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8

WORKDIR /src
COPY . .

RUN make


FROM ubuntu:24.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    libstdc++6 \
    libssl3t64 \
    && rm -rf /var/lib/apt/lists/*

ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8

RUN groupadd -g 10001 appgroup \
    && useradd -u 10001 -g appgroup -m -s /usr/sbin/nologin appuser \
    && mkdir -p /data

COPY --from=builder /src/game /usr/local/bin/game
COPY --from=builder /src/config.json /data/config.json

RUN chown -R appuser:appgroup /data \
    && chmod 755 /usr/local/bin/game \
    && chmod 644 /data/config.json

WORKDIR /data
USER appuser

CMD ["game"]
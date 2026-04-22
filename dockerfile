FROM debian:bookworm AS builder

RUN apt-get update && apt-get install -y \
  build-essential \
  libcjson-dev \
  make \
  && rm -rf /var/lib/apt/lists/*
WORKDIR /app

COPY . .
RUN make release

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y \
  libcjson1 && rm -rf /var/lib/apt/lists/*
COPY --from=builder /app/bin/miniweb /usr/local/bin/miniweb

RUN mkdir -p /var/www
WORKDIR /var/www

EXPOSE 8080
ENTRYPOINT ["miniweb"]
CMD ["--root", "/var/www"]

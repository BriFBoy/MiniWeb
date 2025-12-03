FROM gcc:latest AS builder

WORKDIR /app

COPY . .

RUN make build

FROM alpine:latest

WORKDIR /app

COPY --from=builder /app/bin/program /app/program

CMD ["./program"]
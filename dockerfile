# Stage 1: Build the C application
FROM gcc:latest AS builder

WORKDIR /app

COPY hello.c .

RUN gcc -o hello hello.c

# Stage 2: Create a minimal image for running the application
FROM alpine:latest

WORKDIR /app

COPY --from=builder /app/hello .

CMD ["./hello"]
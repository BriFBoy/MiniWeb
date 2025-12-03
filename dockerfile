FROM gcc:latest AS builder

WORKDIR /WORK

COPY . .

# Ensure build directories exist, then run the Makefile's `build` target
RUN mkdir -p bin obj && make build

FROM debian:stable-slim

WORKDIR /WORK

# Copy the entire build output from the builder's /app into the runtime image
# Use uppercase COPY and copy the directory root to preserve structure
COPY --from=builder /WORK/app/* /WORK/app/

COPY --from=builder /WORK/bin/program /WORK/program

RUN chmod +x /WORK/program

CMD ["./program"]
FROM archlinux:latest AS builder

WORKDIR /WORK

COPY . .

RUN pacman -Sy --noconfirm make gcc && mkdir -p bin obj && make build


FROM archlinux:latest

WORKDIR /WORK

COPY --from=builder /WORK/app/* /WORK/app/
COPY --from=builder /WORK/bin/program /WORK/program

ENV MINIWEB_SOURCE=./app

CMD ["./program"]

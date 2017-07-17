FROM alpine:latest

COPY . /src

RUN apk update && \
    apk add --no-cache glib && \
    apk add --no-cache --virtual depends \
        make \
        musl-dev \
        glib-dev \
        gcc && \
    cd /src && \
    make install && \
    cd / && \
    rm -rf /src && \
    apk del --virtual depends

ENTRYPOINT ["/usr/local/bin/clangsay"]

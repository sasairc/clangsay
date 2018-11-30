# build stage
FROM alpine:3.8 as build

COPY . /src
WORKDIR /src

RUN apk update
RUN apk add gcc make pkgconf glib-dev musl-dev
RUN make WITH_GLIB=0 WITH_REGEX=0 WITH_GNU=0 install-bin install-cows

# runtime stage
FROM alpine:3.8

COPY --from=build /usr/local/bin/clangsay /usr/local/bin/clangsay
COPY --from=build /usr/local/share/clangsay/cows /usr/local/share/clangsay/cows

ENTRYPOINT ["/usr/local/bin/clangsay"]

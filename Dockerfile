FROM alpine:3.11

# setup APK
RUN apk update && apk upgrade \
    && apk add --no-cache ca-certificates \
    && rm -rf /var/cache/apk/* \
# install core tools
    && apk add --no-cache libc-dev gcc make wget\
# install bash for convenience
   && apk add --no-cache bash \
# install git to fetch submodules
   && apk add --no-cache git \
# install buildsystem
   && apk add --no-cache cmake


# install clang (g++ is needed for libstdc++)
RUN apk update && apk add --no-cache g++ clang

# setup clang as compiler
ENV CC  clang
ENV CXX clang++

WORKDIR /IRC
COPY ./* ./

RUN make re

ENTRYPOINT ["./server"]
CMD ["6667","gautier"]


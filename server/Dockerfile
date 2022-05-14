FROM ubuntu:latest

RUN apt-get update \
    && apt-get -y -q install net-tools build-essential apt-utils

COPY /src/server.c /home/server.c

ADD wrapper /usr/bin/

RUN chmod a+x /usr/bin/wrapper

EXPOSE 9666/tcp

# Run eternal loop
ENTRYPOINT ["wrapper"]
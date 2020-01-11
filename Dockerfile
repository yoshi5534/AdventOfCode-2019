FROM debian:10

ENV workspace="/path/to/workspace"

# Let's start with some basic stuff.
RUN apt-get update -qq && apt-get install -qqy \
  apt-transport-https \
  ca-certificates \
  curl \
  lxc \
  iptables

# Install Docker from Docker Inc. repositories.
RUN curl -sSL https://get.docker.com/ | sh

WORKDIR /workspaces
ADD .devcontainer/ container

CMD ["/bin/bash", "-c", "docker build -t yoshi5534/aoc2019 container  && docker run -v ${workspace}:/workspaces/aoc2019 --cap-add=SYS_PTRACE --security-opt seccomp=unconfined yoshi5534/aoc2019 /bin/bash /workspaces/aoc2019/build.sh"]
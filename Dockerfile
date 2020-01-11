FROM debian:10

# This is the workspace directory *outside* of the container 
# in order to access the build results.
ENV workspace="/workspaces/aoc2019"

# We create a new user with sudo rights.
# Change the ids if they don't match to yours.
ARG USERNAME=aoc2019
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && apt-get update \
    && apt-get install -y sudo \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

USER $USERNAME

# Let's start with some basic stuff.
RUN sudo apt-get update -qq && sudo apt-get install -qqy \
  apt-transport-https \
  ca-certificates \
  curl \
  lxc \
  iptables

# Install Docker from Docker Inc. repositories.
RUN curl -sSL https://get.docker.com/ | sh

RUN sudo usermod -aG docker $USERNAME

WORKDIR /workspaces
ADD . aoc2019

# Build the devcontainer and run a release build.
CMD ["/bin/bash", "-c", "docker build -t yoshi5534/aoc2019 aoc2019/.devcontainer && docker run -v ${workspace}:/workspaces/aoc2019 --cap-add=SYS_PTRACE --security-opt seccomp=unconfined yoshi5534/aoc2019 /bin/bash /workspaces/aoc2019/build.sh"]

# Run with: docker run -v /var/run/docker.sock:/var/run/docker.sock -e workspace=`pwd` run_devcontainer
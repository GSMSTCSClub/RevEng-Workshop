FROM ubuntu:latest
RUN apt-get update
RUN apt-get install openssh-server wget curl net-tools build-essential vim less unzip unrar nano -y
RUN echo 'PermitRootLogin yes' > /etc/ssh/sshd_config.d/root.conf
RUN sed -i -e "/Port /c\Port 3032" /etc/ssh/sshd_config
RUN echo 'root:yyds' | chpasswd
RUN useradd -m -s /bin/bash hacker
RUN echo 'hacker:cyberdragons' | chpasswd
EXPOSE 3032
ENTRYPOINT service ssh start && /bin/bash

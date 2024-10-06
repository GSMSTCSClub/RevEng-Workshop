# CyberDragons Reverse Engineering Workshop

![image](https://github.com/user-attachments/assets/e3357abd-13cb-42d9-add4-187fd80029a0)

## Overview
This workshop was designed to help students develop skills in reverse engineering and cybersecurity by engaging with practical challenges. It's built using CTFd for CTF competitions, including remote debugging and a web shell interface to simulate real-world attack/defense scenarios. The workshop was going to serve as the foundation for KoanCTF, a CTF competition for high schoolers sponsored by Google for infra, but we didn't have time to realize it.

## Prerequisites
- **Docker** and **Docker Compose** installed on the host machine
- **Nginx** installed for reverse proxy and SSL support
- **Certbot** for managing SSL certificates
- **GitHub Student Education plan** (or a domain provider of your choice) to obtain a domain name

## Setup Instructions

### 1. **Clone the Repository**
Clone the shared GitHub repository containing all challenge files and Docker server files:
```bash
git clone https://github.com/GSMSTCSClub/RevEng-Workshop
cd RevEng-Workshop
```

### 2. **CTFd Docker Setup**
The workshop is hosted on **CTFd**. To run it, follow these steps:

1. **Modify the `docker-compose.yml`** to include Nginx and the necessary certificates.
   
   Ensure the `ctfd` service has the following configuration:
   ```yaml
   version: '3'
   services:
     ctfd:
       image: ctfd/ctfd
       ports:
         - "80:8000"
         - "443:443"
       volumes:
         - ./Challenges:/opt/CTFd/CTFd/challenges
         - ./Platform/conf/nginx:/etc/ssl/certs
         - ./path-to-nginx-conf:/etc/nginx/nginx.conf # You have to create an NGINX configuration file and point to it!
   ```

2. **Start the Docker containers:**
   ```bash
   cd Platform
   sudo docker-compose up -d
   ```

3. **Verify Nginx Configuration:** Make sure your SSL certificates are set correctly in `nginx.conf`. Redirect HTTP to HTTPS by adding the following in your Nginx config:
   ```nginx
   server {
       listen 80;
       server_name cyberdragons.me www.cyberdragons.me;
       return 301 https://$host$request_uri;
   }
   
   server {
       listen 443 ssl;
       ssl_certificate /etc/ssl/certs/fullchain.pem;
       ssl_certificate_key /etc/ssl/private/privkey.pem;
       ...
   }
   ```

4. **Enable SSL with Certbot:** Generate SSL certificates for your domain with Let's Encrypt:
   ```bash
   sudo certbot --nginx -d cyberdragons.me -d www.cyberdragons.me
   ```

### 3. **MariaDB Caching**
To improve the performance of the CTFd instance, ensure MariaDB caching is enabled:
```bash
[mysqld]
query_cache_size = 16M
query_cache_type = 1
```
Restart MariaDB:
```bash
sudo systemctl restart mariadb
```

### 4. **DNS Configuration**
Point your domain’s DNS to the public IP address of your server (e.g., using Namecheap or other providers). Follow your DNS provider's instructions to set an `A` record pointing to your IP.

### 5. **Remote Debugging Environment**
For reverse engineering challenges that require remote interaction, you should use **redjail** to sandbox challenge instances. To accomplish this, I used a Dockerfile to describe how the container is built and Docker Compose to manage the service for that container. For actual CTF infra deployment, it's better to use Docker Swarm than having a separate service for every container; however, this is vastly more complicated and outside the scope of this repository. It's also significantly better to create a README.md file for every challenge and have CTFd automatically recognize it for every challenge. Unfortunately, I created the challenges first and assigned them descriptions retroactively, so this wasn't applicable. Additionally, **make sure you don't have any conflicting ports for your challenges!** Docker would give you an error about a port already being binded if you tried. Here's an example of my Dockerfile and Docker Compose file for the challenge "Forgotten Password":

#### Dockerfile
```
FROM pwn.red/jail
COPY --from=ubuntu / /srv
COPY forgotten-password /srv/app/run
```

#### docker-compose.yml
```yml
version: '2.4'
services:
  shell:
    build: .
    ports:
      - 5000:5000
    privileged: true
```

#### Challenge README 
Again, this was never implemented per-challenge basis.
```
Oh no! My grandma Suzan forgot her password for an
important applcation! Can you analyze the source code for the
application and help her get her password back?

A stripped-down version of the source code is available for
download. To run the binary, please run nc live-opt.live 5000
```

### 6. **WeTTY Web Shell Setup**
During the workshop, I was able to provide students with web-based terminal access using WeTTY. WeTTY provides a shell in the browser using SSH and, although a Dockerfile for WeTTY is not included in this repo, it is pretty easy to get it set up:

1. **Deploy WeTTY container:**
   ```bash
   docker run -d --name wetty -p 3000:3000 wettyoss/wetty --sslkey /path-to/privatekey --sslcert /path-to/fullchain
   ```

2. **Login Configuration:** Configure the `hacker` user account with restricted permissions and SSH access on port `23810`.

3. **Test the setup:** Open a browser and navigate to `http://cyberdragons.me:3000`. Log in using the `hacker` user credentials you configured via SSH.

### 7. **Challenge Deployment**
Challenges can either be uploaded to the CTFd platform or hosted as part of the Docker Swarm (for binaries requiring remote debugging). To add challenges to CTFd:

1. Log in to the CTFd admin panel.
2. Navigate to **Challenges** > **New Challenge**.
3. Provide the challenge description, point value, and any necessary downloadable files or network access information.

For reverse engineering challenges requiring remote debugging, configure your challenge to include a netcat command, e.g.:
```bash
nc <public-ip> <port>
```

### 8. **Issues & Troubleshooting**
- If the domain is inaccessible, it might be due to DNS propagation delays.
- For email issues (e.g., password recovery), consider using an SMTP service like SendGrid or Mailgun.

To install simply run:
./install.sh

example:
./install.sh
./AkamiProject

To install docker simply install the docker and link port 8080

example:
sudo docker build -t akamiproject . --progress plain
sudo docker run -d -p 127.0.0.1:8080:8080 akamiproject

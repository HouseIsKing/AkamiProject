FROM ubuntu:latest
LABEL authors="shiningblack"
WORKDIR /app
RUN apt-get update
RUN apt-get install cmake -y
RUN apt-get install g++ -y
COPY . .
RUN ./install.sh
CMD ["./AkamiProject"]
EXPOSE 8080

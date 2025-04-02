FROM ubuntu:latest
LABEL authors="shiningblack"
WORKDIR /app
COPY . .
RUN apt-get update
RUN apt-get install cmake -y
RUN ./install.sh
CMD ["./AkamiProject"]
EXPOSE 8080

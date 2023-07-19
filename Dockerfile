# Get the base Ubuntu image from Docker Hub
FROM ubuntu:latest
# Update required apps on the base image
RUN apt-get -y update && apt-get install -y
RUN apt-get -y install g++ cmake
# Copy the current folder which contains C++ source code to the Docker image
COPY . .
# Specify the working directory
WORKDIR .

EXPOSE 8000

# Create build dir, configure CMake and run build
RUN mkdir build
RUN cmake -B build -S .
RUN cmake --build build
# Run the built application
CMD ./build/WebServer
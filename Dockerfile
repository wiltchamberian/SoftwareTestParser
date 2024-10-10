# Use an official Ubuntu as a base image
FROM alpine:latest

#Install essential tools and packages
RUN apk add --no-cache g++ make cmake

# Copy the source code to the container
WORKDIR /app
COPY . .

# Build the C++ program
RUN g++ -o run_program run.cpp -std=c++11

# Set the entrypoint to the program
CMD ["./run_program"]

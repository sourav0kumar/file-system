# Use an official C++ runtime as a parent image
FROM gcc:latest

# Set the working directory to /app
WORKDIR /app

# Copy the current directory contents into the container at /app
COPY . /app

# Compile the C++ code
RUN g++ -o try try.cpp

# Define environment variable
ENV NAME try

# Run the compiled program when the container launches
CMD ["./try"]

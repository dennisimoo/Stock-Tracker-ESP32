# Use official Python runtime as base image
FROM python:3.11-slim

# Set working directory in container
WORKDIR /app

# Copy the Stock_Tracker_webflasher directory into the container
COPY Stock_Tracker_webflasher/ .

# Expose port 8003
EXPOSE 8003

# Run the HTTP server on port 8003
CMD ["python3", "-m", "http.server", "8003"]
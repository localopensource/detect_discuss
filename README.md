# Lane Detection with AI Scene Description

This project demonstrates a computer vision pipeline for detecting lane lines in driving videos and combines it with a locally running AI model to generate simple, human-readable descriptions of the road scene.

The goal of this project is to explore how traditional image processing and modern language models can work together in a driving assistance context.

Repository: [https://github.com/localopensource/detect_discuss.git](https://github.com/localopensource/detect_discuss.git)

## Demo

A short demonstration of the project can be found here:

[https://youtu.be/SYUnABLPq8o](https://youtu.be/SYUnABLPq8o)

## Overview

The system processes a driving video frame by frame and detects lane markings using classical computer vision techniques. After detecting the lanes, it summarizes the road condition and sends this information to a local AI model (Ollama), which generates a natural language explanation of what is happening on the road.

This creates a simple pipeline that connects visual perception with language understanding.

## Key Features

* Real-time lane detection using OpenCV
* Edge detection and Hough Transform for line extraction
* Region of interest filtering to focus on the road
* Overlay of detected lane lines on video frames
* AI-generated scene descriptions using a local Ollama model
* Periodic analysis of video frames (every few seconds)

## Requirements

To build and run this project, you will need:

* C++17 or later
* CMake 3.10 or higher
* OpenCV library
* libcurl
* Ollama installed and running locally

## Setup Instructions

Start by cloning the repository:

git clone [https://github.com/localopensource/detect_discuss.git](https://github.com/localopensource/detect_discuss.git)
cd detect_discuss

Make sure all dependencies are installed correctly. In particular, OpenCV and libcurl must be available in your system environment. Also ensure that Ollama is installed and running.

## Build

Create a build directory and compile the project:

mkdir build
cd build
cmake ..
make

## Run

Before running the application, start the local AI model:

ollama run llama3.2

Then execute the program:

./lanedetection

A window will open showing lane detection results in real time.

At regular intervals, the system will also print AI-generated descriptions of the current road scene in the terminal.

## Input Video

The input video is currently defined in the source code as:

cv::VideoCapture cap("C:\Users\WINDOWS\projects\Lane_Detection\road.mp4");

You can modify this path to use:

* A different recorded driving video
* Your own dataset
* A webcam feed (if supported by OpenCV)

## How It Works

The pipeline follows these steps:

1. The video is loaded frame by frame
2. Each frame is converted to grayscale
3. Gaussian blur is applied to reduce noise
4. Canny edge detection identifies edges
5. A region of interest isolates the road area
6. Hough Line Transform detects lane markings
7. Detected lines are drawn over the original frame
8. A simple rule-based summary is generated
9. The summary is sent to a local AI model via HTTP
10. The AI returns a natural language explanation

## Project Structure

* detect.cpp – main implementation for lane detection and AI integration
* CMakeLists.txt – build configuration

## Build Configuration

cmake_minimum_required(VERSION 3.10)
project(lanedetection_ai)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(CURL REQUIRED)

add_executable(lanedetection detect.cpp)

target_link_libraries(lanedetection
${OpenCV_LIBS}
CURL::libcurl
)

## Notes

* Ensure Ollama is running before executing the program
* The model used is llama3.2:latest, but it can be changed easily in the code
* Results depend on lighting conditions and video quality

## Future Improvements

This project can be extended in several ways:

* More stable lane tracking across frames
* Curved lane detection for complex roads
* Real-time webcam or dashcam integration
* Improved AI prompts for better contextual understanding
* Structured output (JSON-based AI responses)
* Performance optimization for real-time systems

## License

This project is released under the MIT License and is free to use and modify.

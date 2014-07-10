OPENCV_DIR = `pkg-config --cflags opencv` -I/usr/include/opencv2
OPENCV_LIBS = `pkg-config --libs opencv`

all : imagify

imagify: face_refinery.o imagify.o
	g++ -Wall -ggdb $(OPENCV_LIBS) -o imagify imagify.o face_refinery.o

face_refinery.o : face_refinery.cpp face_refinery.hpp
	g++ -Wall -ggdb $(OPENCV_DIR) -c face_refinery.cpp

imagify.o : imagify.cpp
	g++ -Wall -ggdb $(OPENCV_DIR) -c imagify.cpp

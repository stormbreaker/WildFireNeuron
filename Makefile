# Makefile for OpenGL/GLUT programs.

# Author: John M. Weiss, Ph.D.
# Written Fall 2014 for CSC433/533 Computer Graphics.

# Usage:  make target1 target2 ...

#-----------------------------------------------------------------------

# GNU C/C++ compiler and linker:
CC = gcc
CXX = g++
LINK = g++

# Turn on optimization and warnings (add -g for debugging with gdb):
# CPPFLAGS = 		# preprocessor flags
CFLAGS = -g -O -Wall
CXXFLAGS = -g -std=c++11 -O -Wall

# OpenGL/Mesa libraries for Linux:
#GL_LIBS = -lglut -lGLU -lGL -lm

#VPATH = src

# OpenGL libraries for Windows (MinGW):
# GL_LIBS = -lglut32 -lglu32 -lopengl32

# OpenGL libraries for Windows (MSVS):
# GL_LIBS = opengl32.lib glu32.lib glut32.lib

#-----------------------------------------------------------------------

# MAKE allows the use of "wildcards", to make writing compilation instructions
# a bit easier. GNU make uses $@ for the target and $^ for the dependencies.

all:   ANNtrain ANNtest CrossValidate

# specific targets
ANNtrain:	main_train.o Parse_Files.o Neuron_Layer.o Normalization.o Randomize.o
	$(LINK) -o $@ $^ 

ANNtest:	main_test.o Parse_Files.o Neuron_Layer.o Normalization.o Randomize.o
	$(LINK) -o $@ $^

CrossValidate: main_cross_validate.o Parse_Files.o Neuron_Layer.o Normalization.o Randomize.o
	$(LINK) -o $@ $^

# generic C and C++ targets for OpenGL programs consisting of only one file
# type "make filename" (no extension) to build
.c:
	$(CC) -o $@ $@.c $(GL_LIBS)

.cpp:
	$(CXX) -o $@ $@.cpp $(GL_LIBS)

# utility targets
clean:
	rm -f *.o *~ core

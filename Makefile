# Copyright (C) 2018 Gian Luigi Romita. All rights reserved.

# TO COMPILE => CREATE SYMBOLIC LINK TO THE OPENSSL FOLDER (eg. 'ln -s /your_openssl_path openssl')

CC = g++

#OPTIMIZE 	= -c -std=c++0x -g -Werror -Woverloaded-virtual -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wswitch-default -Wundef -Weffc++ -Woverloaded-virtual
OPTIMIZE 	=  -c -std=c++0x -g 

CFLAGS 		=  $(OPTIMIZE)

INCLUDES	= -I include/ -I /usr/include -I /usr/include/opencv2
LIBS		= -L opencv  
LIBS		+=  -lopencv_calib3d /usr/lib/x86_64-linux-gnu/libopencv_contrib.so -lopencv_contrib /usr/lib/x86_64-linux-gnu/libopencv_core.so -lopencv_core /usr/lib/x86_64-linux-gnu/libopencv_features2d.so -lopencv_features2d /usr/lib/x86_64-linux-gnu/libopencv_flann.so -lopencv_flann /usr/lib/x86_64-linux-gnu/libopencv_gpu.so -lopencv_gpu /usr/lib/x86_64-linux-gnu/libopencv_highgui.so -lopencv_highgui /usr/lib/x86_64-linux-gnu/libopencv_imgproc.so -lopencv_imgproc /usr/lib/x86_64-linux-gnu/libopencv_legacy.so -lopencv_legacy /usr/lib/x86_64-linux-gnu/libopencv_ml.so -lopencv_ml /usr/lib/x86_64-linux-gnu/libopencv_objdetect.so -lopencv_objdetect /usr/lib/x86_64-linux-gnu/libopencv_ocl.so -lopencv_ocl /usr/lib/x86_64-linux-gnu/libopencv_photo.so -lopencv_photo /usr/lib/x86_64-linux-gnu/libopencv_stitching.so -lopencv_stitching /usr/lib/x86_64-linux-gnu/libopencv_superres.so -lopencv_superres /usr/lib/x86_64-linux-gnu/libopencv_ts.so -lopencv_ts /usr/lib/x86_64-linux-gnu/libopencv_video.so -lopencv_video /usr/lib/x86_64-linux-gnu/libopencv_videostab.so -lopencv_videostab -lpthread  

EXECUTABLE	= dsp 

#----- OBJECTS TO BUILD ------
OBJECTS_MAIN= \
	dsp.o 
	
OBJECTS= \
	LaneDetection.o \
	perspective.o

.PHONY: all clean

all: $(OBJECTS) $(OBJECTS_MAIN) $(EXECUTABLE)

clean: 
	rm -f $(OBJECTS)
	rm -f $(OBJECTS_MAIN)
	rm -f $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS_MAIN) $(OBJECTS)
	$(CC) $(OBJECTS_MAIN) $(OBJECTS) $(LIBS) $(TARGETARCH) $(LIBTOOLNAME) -o $@

dsp.o : src/dsp.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<		
LaneDetection.o : src/LaneDetection.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<
ObjectDetection.o : src/ObjectDetection.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<
perspective.o : src/perspective.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $<


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS =  -O3 -Wall -fexceptions
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = 

INC_DEBUG =  $(INC)
CFLAGS_DEBUG =  $(CFLAGS) -std=c++0x -g
RESINC_DEBUG =  $(RESINC)
RCFLAGS_DEBUG =  $(RCFLAGS)
LIBDIR_DEBUG =  $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG =  $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/schedulingAlgorithm

INC_RELEASE =  $(INC)
CFLAGS_RELEASE =  $(CFLAGS) -O3 -std=c++11 -std=c++0x
RESINC_RELEASE =  $(RESINC)
RCFLAGS_RELEASE =  $(RCFLAGS)
LIBDIR_RELEASE =  $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE =  $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = bin/Release/schedulingAlgorithm

OBJ_DEBUG = $(OBJDIR_DEBUG)/DataLogger.o $(OBJDIR_DEBUG)/FirstComeFirstServe.o $(OBJDIR_DEBUG)/Process.o $(OBJDIR_DEBUG)/RoundRobin.o $(OBJDIR_DEBUG)/ShortestJobFirst.o $(OBJDIR_DEBUG)/ShortestJobFirstWithExponentialAveraging.o $(OBJDIR_DEBUG)/ShortestRemainingJobFirst.o $(OBJDIR_DEBUG)/main.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/DataLogger.o $(OBJDIR_RELEASE)/FirstComeFirstServe.o $(OBJDIR_RELEASE)/Process.o $(OBJDIR_RELEASE)/RoundRobin.o $(OBJDIR_RELEASE)/ShortestJobFirst.o $(OBJDIR_RELEASE)/ShortestJobFirstWithExponentialAveraging.o $(OBJDIR_RELEASE)/ShortestRemainingJobFirst.o $(OBJDIR_RELEASE)/main.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/DataLogger.o: DataLogger.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c DataLogger.cpp -o $(OBJDIR_DEBUG)/DataLogger.o

$(OBJDIR_DEBUG)/FirstComeFirstServe.o: FirstComeFirstServe.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c FirstComeFirstServe.cpp -o $(OBJDIR_DEBUG)/FirstComeFirstServe.o

$(OBJDIR_DEBUG)/Process.o: Process.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c Process.cpp -o $(OBJDIR_DEBUG)/Process.o

$(OBJDIR_DEBUG)/RoundRobin.o: RoundRobin.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c RoundRobin.cpp -o $(OBJDIR_DEBUG)/RoundRobin.o

$(OBJDIR_DEBUG)/ShortestJobFirst.o: ShortestJobFirst.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ShortestJobFirst.cpp -o $(OBJDIR_DEBUG)/ShortestJobFirst.o

$(OBJDIR_DEBUG)/ShortestJobFirstWithExponentialAveraging.o: ShortestJobFirstWithExponentialAveraging.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ShortestJobFirstWithExponentialAveraging.cpp -o $(OBJDIR_DEBUG)/ShortestJobFirstWithExponentialAveraging.o

$(OBJDIR_DEBUG)/ShortestRemainingJobFirst.o: ShortestRemainingJobFirst.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ShortestRemainingJobFirst.cpp -o $(OBJDIR_DEBUG)/ShortestRemainingJobFirst.o

$(OBJDIR_DEBUG)/main.o: main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.cpp -o $(OBJDIR_DEBUG)/main.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/DataLogger.o: DataLogger.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c DataLogger.cpp -o $(OBJDIR_RELEASE)/DataLogger.o

$(OBJDIR_RELEASE)/FirstComeFirstServe.o: FirstComeFirstServe.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c FirstComeFirstServe.cpp -o $(OBJDIR_RELEASE)/FirstComeFirstServe.o

$(OBJDIR_RELEASE)/Process.o: Process.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c Process.cpp -o $(OBJDIR_RELEASE)/Process.o

$(OBJDIR_RELEASE)/RoundRobin.o: RoundRobin.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c RoundRobin.cpp -o $(OBJDIR_RELEASE)/RoundRobin.o

$(OBJDIR_RELEASE)/ShortestJobFirst.o: ShortestJobFirst.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ShortestJobFirst.cpp -o $(OBJDIR_RELEASE)/ShortestJobFirst.o

$(OBJDIR_RELEASE)/ShortestJobFirstWithExponentialAveraging.o: ShortestJobFirstWithExponentialAveraging.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ShortestJobFirstWithExponentialAveraging.cpp -o $(OBJDIR_RELEASE)/ShortestJobFirstWithExponentialAveraging.o

$(OBJDIR_RELEASE)/ShortestRemainingJobFirst.o: ShortestRemainingJobFirst.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ShortestRemainingJobFirst.cpp -o $(OBJDIR_RELEASE)/ShortestRemainingJobFirst.o

$(OBJDIR_RELEASE)/main.o: main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c main.cpp -o $(OBJDIR_RELEASE)/main.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release


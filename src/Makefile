# Instruction for users:
# Replace the value of the USER_CXXFLAGS variable with avrocpp and Poco::Net include paths
# (the current one covers both for me, but it might be different for you)
USER_CXXFLAGS = -I/opt/gaia-dev-libs/include/ 

#=============================================================================
# The user should not need to change anything below:
#=============================================================================

CXX = g++

INCDIRS = -I. -I./obj_defs -I./Utils $(USER_CXXFLAGS)
CXXFLAGS =  -DNO_STORAGE -Wall -DDEBUG_BUILD


VPATH = Utils:obj_defs

LIB_PATH = ../lib/
LIB_NAME = libgpudb.so

# Default target executed when no arguments are given to make.
default_target: $(LIB_NAME)
.PHONY : default_target

#=============================================================================

SRCS =  $(wildcard *.cpp) $(wildcard Utils/*.cpp) $(wildcard obj_defs/*.cpp) 
OBJS = $(SRCS:.cpp=.o)

#=============================================================================

$(LIB_NAME): $(OBJS)
	$(CXX) -g -shared -fPIC -Wl,-soname,$@ -o $(LIB_PATH)$@ $^


%.o : %.cpp
	$(CXX) $(CXXFLAGS) -DLOG_OUTPUT -fPIC -o $@ -c $(INCDIRS) $<


test_gpudb.cpp: GPUdb.h
AvroTypes.cpp: AvroTypes.h
AvroUtils.cpp: AvroUtils.h AvroTypes.h
GPUdb.cpp: GPUdb.h HTTPUtils.h AvroUtils.h GPUdbExceptions.h
HTTPUtils.cpp: HTTPUtils.h AvroUtils.h
obj_defs.cpp: obj_defs.h

AvroUtils.o : AvroUtils.cpp AvroUtils.h AvroTypes.h
	$(CXX) $(CXXFLAGS) $< -o $@



clean:
	rm -rf *.o */*.o *.po */*.po

.PHONY: clean

clean-all:
	rm -rf *.o */*.o *.po */*.po ../lib/$(LIB_NAME) *.so

.PHONY: clean-all


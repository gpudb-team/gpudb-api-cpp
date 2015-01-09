# Instruction for users:
# Replace the value of the USER_CXXFLAGS and USER_LDFLAGS variables with avrocpp
# and boost include and library paths.
# (the current ones covers both for me, but it might be different for you)

# Avro and boost include paths: -I/path/to/avro/include  -I/path/to/boost/include
USER_CXXFLAGS =  -I/opt/gaia-dev-libs/include/
# Avro and boost library paths: -L/path/to/avro/lib -L/path/to/boost/lib
USER_LDFLAGS = -L/opt/gaia-dev-libs/lib

# Additional instructions:
# Need to add the paths to boost, avro, and gpudb (./lib or wherever you copy it)
# libraries to environment variable LD_LIBRARY_PATH at the shell prompt thusly:
# >export LD_LIBRARY_PATH=/path/to/avrolib:/path/to/boostlib:/path/to/gpulib:$LD_LIBRARY_PATH


#=============================================================================
# The user should not need to change anything below:
#=============================================================================


# Default target executed when no arguments are given to make.
default_target: test_gpudb
.PHONY : default_target


#=============================================================================

CXX = g++

INCDIRS = -I. -I./src/
CXXFLAGS = -Wall -g $(INCDIRS) $(USER_CXXFLAGS)

LIBDIRS = -L./lib $(USER_LDFLAGS)
LDFLAGS = $(LIBDIRS) $(USER_LDFLAGS) -lboost_system -lboost_filesystem -lboost_program_options -lavrocpp -lgpudb


#=============================================================================

SRCS =  $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

#=============================================================================

test_gpudb: $(OBJS)
	$(CXX) $(CXXFLAGS) $(^F) $(LDFLAGS) -o $@


.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<



clean:
	rm -rf *.o */*.o test_gpudb

.PHONY: clean

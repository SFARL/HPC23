CXX = g++
CXXFLAGS = -std=c++11 -O3 -march=native -g
GCC_VERSION = gcc-9.2


RM = rm -f
MKDIRS = mkdir -p

TARGETS = $(basename $(wildcard *.cpp))

all : $(TARGETS)
    module load $(GCC_VERSION)

%:%.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	$(RM) $(TARGETS)


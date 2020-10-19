TARGET = cal_T

SRCS = $(TARGET).cpp
OBJS = $(TARGET).o

ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIBS   = $(shell root-config --libs)
ROOTGLIBS = $(shell root-config --glibs)

CXXFLAGS   = $(ROOTCFLAGS) -Wall -fPIC
CXXLIBS    = $(ROOTLIBS)
CC = g++ 

$(TARGET): $(OBJS)
	$(CC) $(CXXLIBS) $(OBJS) -o $@

# suffix rule
.cpp.o:
	$(CC) $(CXXFLAGS) -c $<

# clean
clean:
	rm -f $(TARGET) $(OBJS)

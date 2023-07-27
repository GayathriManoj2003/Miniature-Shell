CXX := g++
CXXFLAGS := -std=c++17

SRCS := shell.cpp
OBJS := $(SRCS:.cpp=.o)

TARGET := gpp

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
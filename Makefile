CXX := g++
CXXFLAGS := -std=c++17

SRCS := tokens.cpp commands.cpp simpleshell.cpp main.cpp
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
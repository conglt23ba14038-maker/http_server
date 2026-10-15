CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic
TARGET := server.exe

SOURCES := Networking/socket.cpp \
           Networking/bindingsocket.cpp \
           Networking/listeningsocket.cpp \
           Networking/connectingsocket.cpp \
           Servers/server.cpp \
           Servers/testserver.cpp \
           Servers/test.cpp

HEADERS := $(wildcard Networking/*.hpp Servers/*.hpp)

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) -lws2_32

run: $(TARGET)
	.\$(TARGET)

clean:
	if exist $(TARGET) del $(TARGET)
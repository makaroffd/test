CC=g++
CFLAGS=-c -Wall -std=c++14
LDFLAGS=-pthread
SOURCES=test2.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test2

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o test2

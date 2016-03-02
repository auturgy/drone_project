EXEC = server_lib
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h*)
OBJECTS = $(SOURCES:.cpp=.o)
LFLAGS = 
DEBUG = -D_DEBUG_
FLAGS = -c -std=c++11 -Wall -pedantic $(DEBUG)
CC = g++-5
LIBS = -L/usr/local/Cellar/boost/1.60.0_1/lib -lboost_regex-mt -lboost_filesystem-mt -lboost_thread-mt -lboost_system-mt
INCLUDES = -I/usr/local/Cellar/boost/1.60.0_1/include/

all: $(EXEC)

$(EXEC) : $(OBJECTS)

	$(CC) $(LFLAGS) $(OBJECTS) $(LIBS) -o $(EXEC)
	
%.o: %.cpp $(HEADERS)

	$(CC) $(FLAGS) $(INCLUDES) $< -o $@

clean:

	rm -f $(EXEC) $(OBJECTS)
	
	
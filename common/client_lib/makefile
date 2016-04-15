EXEC = client_lib
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h*)
OBJECTS = $(SOURCES:.cpp=.o)
LFLAGS = 
DEBUG = -g -D_DEBUG_ -Wall -pedantic 
FLAGS = -c -std=c++14 $(DEBUG) -D_CLIENT_TEST_
CC = g++-5
LIBS = -lm -ldl -lpthread -L/usr/local/Cellar/boost/1.60.0/lib -lboost_regex -lboost_filesystem -lboost_thread -lboost_system -lboost_atomic
INCLUDES = -I/usr/local/Cellar/boost/1.60.0/include/

all: $(EXEC)

$(EXEC) : $(OBJECTS)

	$(CC) $(LFLAGS) $(OBJECTS) $(LIBS) -o $(EXEC)
	
%.o: %.cpp $(HEADERS)

	$(CC) $(FLAGS) $(INCLUDES) $< -o $@

clean:

	rm -f $(EXEC) $(OBJECTS)
	
	
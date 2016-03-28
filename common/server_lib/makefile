EXEC = server_lib
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h*)
OBJECTS = $(SOURCES:.cpp=.o)
LFLAGS = 
DEBUG = -g -D_DEBUG_ -D_SERVER_TEST_
FLAGS = -c -std=c++11 -Wall -pedantic $(DEBUG) 
# for OS X
CC = g++-5
LIBS = -lm -ldl -lpthread -L/usr/local/Cellar/boost/1.60.0/lib -lboost_regex -lboost_filesystem -lboost_thread -lboost_system -lboost_atomic
INCLUDES = -I/usr/local/Cellar/boost/1.60.0/include/
# for Raspbian
#CC = g++
#LIBS = -lm -ldl -lpthread -lboost_regex -lboost_filesystem -lboost_thread -lboost_system -lboost_atomic
#INCLUDES = 


all: $(EXEC)

$(EXEC) : $(OBJECTS)

	$(CC) $(LFLAGS) $(OBJECTS) $(LIBS) -o $(EXEC)
	
%.o: %.cpp $(HEADERS)

	$(CC) $(FLAGS) $(INCLUDES) $< -o $@

clean:

	rm -f $(EXEC) $(OBJECTS)
	
	
EXEC = client_lib
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h*)
OBJECTS = $(SOURCES:.cpp=.o)
#FLAGS = -std=c++11 -stdlib=libc++
FLAGS = -std=c++11 -Wall -Wextra -Weffc++ -pedantic $(DEBUG)
#-g -Wall -Werror -Wextra  $(DEBUG)
#FLAGS = -std=c++11 -stdlib=libstdc++ -Wall -Wextra -Weffc++ -pedantic 
#FLAGS = -std=c++0x -c
#FLAGS = -std=gnu++11 -stdlib=libc++ -lstdc++
CC = g++-5

#LIBS = -L/usr/local/Cellar/boost/1.57.0/lib -lboost_regex-mt -lboost_filesystem-mt -lboost_thread-mt -lboost_serialization-mt -lboost_system-mt -lboost_date_time-mt
#INCLUDES = -I/usr/local/Cellar/boost/1.57.0/include/

LIBS = -L/usr/local/Cellar/boost/1.60.0_1/lib -lboost_regex-mt -lboost_filesystem-mt -lboost_thread-mt -lboost_serialization-mt -lboost_system-mt -lboost_date_time-mt -lboost_program_options
INCLUDES = -I/usr/local/Cellar/boost/1.60.0_1/include/

all: $(EXEC)

$(EXEC) : $(OBJECTS)

	$(CC) $(FLAGS) $(OBJECTS) $(LIBS) -o $(EXEC)
		
%.o: %.cpp $(HEADERS)
	$(CC) $(INCLUDES) -c $< -o $@

clean:

	rm -f $(EXEC) $(OBJECTS)
	
	
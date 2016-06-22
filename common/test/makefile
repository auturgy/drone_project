EXEC 		:= test

OBJ_DIR 	:= obj

SOURCES 	:= $(wildcard *.cpp)
HEADERS 	:= $(wildcard *.h*)
OBJECTS 	:= $(addprefix $(OBJ_DIR)/, $(SOURCES:.cpp=.o)) 

DEBUG 		:= -g -Wall -pedantic -D_DEBUG_ 
FLAGS 		:= -c -std=c++14 $(DEBUG) -D_DRONE_AGENT_

# for OS X
#CC 		:= g++-5
#LIBS 		= -lm -ldl -lpthread -L/usr/local/Cellar/boost/1.60.0_2/lib -lboost_regex -lboost_filesystem -lboost_thread-mt -lboost_system -lboost_atomic-mt
#INCLUDES 	= -I/usr/local/Cellar/boost/1.60.0_2/include/
# for Raspbian
CC 			:= g++
LIBS 		:= -lm -ldl -lpthread -L/usr/local/opt/opencv3/lib
INCLUDES 	:= -I/usr/local/opt/opencv3/include

all: $(OBJ_DIR) $(EXEC)


$(OBJ_DIR):
	@mkdir -p $@


$(EXEC) : $(OBJECTS)

	$(CC) -o $(EXEC) $^ $(LIBS)


$(OBJECTS) : $(OBJ_DIR)/%.o: %.cpp

	$(CC) $(FLAGS) $(INCLUDES) $< -o $@

clean:

	rm -f $(EXEC) $(OBJECTS) $(LIB_OBJECTS)
	
	
EXEC 		:= drone_agent

OBJ_DIR 	:= obj

SOURCES 	:= $(wildcard *.cpp)
HEADERS 	:= $(wildcard *.h*)
OBJECTS 	:= $(addprefix $(OBJ_DIR)/, $(SOURCES:.cpp=.o)) 

LIB_SRC_DIR := ../common/client_lib

LIB_SOURCES := $(notdir $(wildcard $(LIB_SRC_DIR)/*.cpp))
LIB_HEADERS := $(notdir $(wildcard $(LIB_SRC_DIR)/*.hpp))
LIB_OBJECTS	:= $(addprefix $(OBJ_DIR)/, $(LIB_SOURCES:.cpp=.o)) 

DEBUG 		:= -g -Wall -pedantic -D_DEBUG_ 
FLAGS 		:= -c -std=c++14 $(DEBUG) -D_DRONE_AGENT_

# for OS X
CC 			:= g++-5
LIBS 		:= -lm -ldl -lpthread -L/usr/local/Cellar/boost/1.60.0/lib -lboost_regex -lboost_filesystem -lboost_thread -lboost_system -lboost_atomic
INCLUDES 	:= -I/usr/local/Cellar/boost/1.60.0/include/
# for Raspbian
#CC 		:= g++
#LIBS 		:= -lm -ldl -lpthread -lboost_regex -lboost_filesystem -lboost_thread -lboost_system -lboost_atomic
#INCLUDES 	:= 

all: $(OBJ_DIR) $(EXEC)


$(OBJ_DIR):
	@mkdir -p $@


$(EXEC) : $(OBJECTS) $(LIB_OBJECTS)

	$(CC) -o $(EXEC) $^ $(LIBS)


$(OBJECTS) : $(OBJ_DIR)/%.o: %.cpp

	$(CC) $(FLAGS) $(INCLUDES) $< -o $@


$(LIB_OBJECTS) : $(OBJ_DIR)/%.o: $(LIB_SRC_DIR)/%.cpp

	$(CC) $(FLAGS) $(INCLUDES) $< -o $@


clean:

	rm -f $(EXEC) $(OBJECTS) $(LIB_OBJECTS)
	
	
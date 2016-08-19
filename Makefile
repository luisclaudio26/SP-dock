CC = g++
FLAGS = -g -O0 -std=c++11
LIBS = -lm -lGL -lglfw -lGLEW $(shell pkg-config --libs gsl)
INC = -I /usr/include/GLFW
EXEC = keypoints

#Get all source files recursively
SRC = $(shell find . -name '*.cpp')

#Generate object filenames from source files
OBJ = $(SRC:%.cpp=%.o)

all: $(OBJ)
	$(CC) $(FLAGS) $(INC) $(OBJ) -o $(EXEC) $(LIBS)

%.o : %.cpp
	$(CC) $(FLAGS) $(INC) -c $< -o $@ $(LIBS)

clean:
	rm $(OBJ)
	rm $(EXEC)

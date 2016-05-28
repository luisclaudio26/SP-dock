CC = g++
CFLAGS = -g -O0 -std=c++11
CLIBS = -lm
EXEC = keypoints

all: graph.o fileio.o unionfind.o node.o vec4.o linalg.o
		$(CC) $(CFLAGS) $(CLIBS) linalg.o vec4.o node.o unionfind.o fileio.o graph.o -o $(EXEC) main.cpp
		make clean

graph.o: src/graph/graph.cpp
		$(CC) $(CFLAGS) $(CLIBS) -c src/graph/graph.cpp

node.o: src/graph/node.cpp
		$(CC) $(CFLAGS) $(CLIBS) -c src/graph/node.cpp

fileio.o: src/io/fileio.cpp
		$(CC) $(CFLAGS) $(CLIBS) -c src/io/fileio.cpp

unionfind.o: src/util/unionfind.cpp
		$(CC) $(CFLAGS) $(CLIBS) -c src/util/unionfind.cpp

vec4.o: src/math/vec4.cpp
		$(CC) $(CFLAGS) $(CLIBS) -c src/math/vec4.cpp	

linalg.o: src/math/linalg.cpp
		$(CC) $(CFLAGS) $(CLIBS) -c src/math/linalg.cpp	

clean:
	rm *.o
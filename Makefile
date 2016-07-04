CC = g++
CFLAGS = -g -O0 -std=c++11
CLIBS = -lm
EXEC = keypoints

all: graph.o fileio.o unionfind.o node.o linalg.o patch.o render.o
		$(CC) $(CFLAGS) $(CLIBS) patch.o linalg.o node.o unionfind.o fileio.o render.o graph.o -o $(EXEC) main.cpp
		make clean

graph.o: src/graph/graph.cpp
		$(CC) $(CFLAGS) $(CLIBS) -c src/graph/graph.cpp

node.o: src/graph/node.cpp
		$(CC) $(CFLAGS) $(CLIBS) -c src/graph/node.cpp

fileio.o: src/io/fileio.cpp
		$(CC) $(CFLAGS) $(CLIBS) -c src/io/fileio.cpp

unionfind.o: src/util/unionfind.cpp
		$(CC) $(CFLAGS) $(CLIBS) -c src/util/unionfind.cpp

linalg.o: src/math/linalg.cpp
		$(CC) $(CFLAGS) $(CLIBS) -c src/math/linalg.cpp	

patch.o: src/graph/patch.cpp
		$(CC) $(CFLAGS) $(CLIBS) -c src/graph/patch.cpp

render.o: src/visualization/render.cpp
		$(CC) $(CLAGS) $(CLIBS) -c src/visualization/render.cpp

clean:
	rm *.o
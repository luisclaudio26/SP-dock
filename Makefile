CC = g++
CFLAGS = -g -O0 -std=c++11
CLIBS = -lm
EXEC = keypoints

all: graph.o fileio.o unionfind.o
		$(CC) $(CFLAGS) $(CLIBS) unionfind.o fileio.o graph.o -o $(EXEC) main.cpp
		make clean

graph.o: src/graph/graph.cpp
		$(CC) $(CFLAGS) $(CLIBS) -c src/graph/graph.cpp

fileio.o: src/io/fileio.cpp
		$(CC) $(CFLAGS) $(CLIBS) -c src/io/fileio.cpp

unionfind.o: src/util/unionfind.cpp
		$(CC) $(CFLAGS) $(CLIBS) -c src/util/unionfind.cpp

clean:
	rm *.o
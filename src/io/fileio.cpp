#include "../../inc/io/fileio.h"
#include <fstream>
#include <sstream>
#include <string>

#include <iostream>

//------------------------------------------
//--------------- INTERNAL -----------------
//------------------------------------------
const unsigned int IGNORE_N = 256;

static void load_vertex(std::string line, Graph& g)
{
	std::stringstream ss(line);

	double x, y, z;
	ss>>x>>y>>z;

	double nx, ny, nz;
	ss>>nx>>ny>>nz;

	g.push_node(x, y, z, nx, ny, nz);
}

static void load_vertice(std::string vert, Graph& g)
{
	std::fstream in;
	in.open(vert, std::fstream::in);

	//Consume first three lines, which are just header info
	in.ignore(IGNORE_N, '\n');
	in.ignore(IGNORE_N, '\n');
	in.ignore(IGNORE_N, '\n');

	while( !in.eof() )
	{
		std::string buffer;
		getline(in, buffer);
		
		load_vertex(buffer, g);
	}

	//Close file
	in.close();
}

static void load_edge(std::string line, Graph& g)
{
	std::stringstream ss(line);

	int v1, v2, v3;
	ss>>v1>>v2>>v3;

	//Indexes inside file are 1-index based
	v1--; v2--; v3--;

	g.push_face(v1, v2, v3);
}

static void load_edges(std::string face, Graph& g)
{
	std::fstream in;
	in.open(face, std::fstream::in);

	//Consume first three lines which are just header info
	in.ignore(IGNORE_N, '\n');
	in.ignore(IGNORE_N, '\n');
	in.ignore(IGNORE_N, '\n');

	while( !in.eof() )
	{
		std::string buffer;
		getline(in, buffer);

		load_edge(buffer, g);
	}

	in.close();
}

//-----------------------------------------------
//--------------- FROM FILEIO.H -----------------
//-----------------------------------------------
FileIO* FileIO::ptr_instance = NULL;

FileIO::FileIO() { }

void FileIO::mesh_from_file(std::string vert, std::string face, Graph& g)
{
	load_vertice(vert, g);
	load_edges(face, g);
}
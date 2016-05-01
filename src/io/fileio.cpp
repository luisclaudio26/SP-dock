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

	g.push_node(x, y, z);
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

	std::cout<<g.graph2str()<<std::endl;;

	//Close file
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
}
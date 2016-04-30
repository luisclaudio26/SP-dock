#ifndef _FILEIO_H_
#define _FILEIO_H_

#include <string>
#include "../graph/graph.h"

class FileIO
{
private:
	static FileIO* ptr_instance;
	FileIO();
public:
	static FileIO* instance() {
		if(!FileIO::ptr_instance)
			FileIO::ptr_instance = new FileIO();
		return FileIO::ptr_instance;
	}

	void mesh_from_file(std::string vert, std::string face, Graph& g);
};

#endif
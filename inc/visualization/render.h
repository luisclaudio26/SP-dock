#ifndef _RENDER_H_
#define _RENDER_H_

#include "../graph/graph.h"
#include <GL/glew.h>
#include <glfw3.h>
#include <vector>

// Fully declared in render.cpp
typedef struct _vertex Vertex;

//Render implemented with a Singleton pattern
class Render
{
private:
	Render();
	static Render *instance_ptr;

	GLFWwindow *window;

	//----------------------------------------
	//--------- internal operations ----------
	//----------------------------------------
	void setup_window();
	void terminate_rendering();
	void draw_geometry_data(const std::vector<Vertex>& geometry_data);

public:
	static Render* instance() {
		if(!Render::instance_ptr)
			Render::instance_ptr = new Render();
		return Render::instance_ptr;
	}

	//------------------------------------------
	//---------- external operations -----------
	//------------------------------------------
	void draw_mesh(Graph& mesh);
	void draw_meshes(Graph& mesh1, Graph& mesh2); //TODO: use variadic function
};

#endif
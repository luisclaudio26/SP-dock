#include "../../inc/visualization/render.h"

#include <iostream>
#include <vector>

typedef struct {
	float x, y, z;
	float nx, ny, nz;
} Vertex;

//Just to not have to type this behemoth in main draw_mesh
#define NODE2VERTEX(n) ((Vertex){n.get_pos()[0], n.get_pos()[1], n.get_pos()[2], n.get_normal()[0], n.get_normal()[1], n.get_normal()[2]})

//---------------------------------------
//----------- From render.h -------------
//---------------------------------------
Render* Render::instance_ptr = 0;

Render::Render()
{

}

void Render::setup_window()
{
	//This will setup a new window and OpenGL context.
	//GLFW or GLUT may be used for this kind of stuff.
	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 1); //Supersampling
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Make sure OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2); //version is 3.0

	//Something about which kind of OpenGL will run here
	//This says to glfw that we're using only OpenGL core functions, not
	//vendor-specific
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create window and set it as the context
	this->window = glfwCreateWindow(800, 600, "Project 1", NULL, NULL);
	glfwMakeContextCurrent(this->window);

	//Initialize GLEW. Don't what the fuck is this.
	glewExperimental = true; //Where the fuck is this coming from?
	glewInit();

	//Get keyboard input
	glfwSetInputMode(this->window, GLFW_STICKY_KEYS, GL_TRUE);

	//Enable z-buffering
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Render::terminate_rendering()
{
	glfwTerminate();
}

void Render::draw_mesh(const Graph& mesh)
{
	//this->setup_window();

	//pack mesh data into vertex buffer
	std::vector<Vertex> vertice;
	const std::vector<Node>& nodes = mesh.get_nodes();
	const std::vector<Face>& faces = mesh.get_faces();

	std::vector<Face>::const_iterator f = faces.begin();
	for( ; f != faces.end(); ++f )
	{
		const Node &f1 = nodes[f->a], 
					&f2 = nodes[f->b], 
					&f3 = nodes[f->c];
	
		vertice.push_back( NODE2VERTEX(f1) );
		vertice.push_back( NODE2VERTEX(f2) );
		vertice.push_back( NODE2VERTEX(f3) );
	}

	//load uniforms

	//main loop
	
	/*
	do
	{
		//Clear screen -> this function also clears stencil and depth buffer
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		//Swap buffer and query events
		glfwSwapBuffers(this->window);
		glfwPollEvents();

	} while(glfwGetKey(this->window, GLFW_KEY_ESCAPE) != GLFW_PRESS && 
			!glfwWindowShouldClose(this->window)); */

	//clean everything
	//this->terminate_rendering();
}
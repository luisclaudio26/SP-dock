#include "../../inc/visualization/render.h"

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
	this->setup_window();

	//load node info

	//load face info

	//load everything into data buffer

	//load uniforms

	//main loop
	do
	{
		//Clear screen -> this function also clears stencil and depth buffer
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		//Swap buffer and query events
		glfwSwapBuffers(this->window);
		glfwPollEvents();

	} while(glfwGetKey(this->window, GLFW_KEY_ESCAPE) != GLFW_PRESS && 
			!glfwWindowShouldClose(this->window));

	//clean everything
	this->terminate_rendering();
}
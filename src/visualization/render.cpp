#include "../../inc/visualization/render.h"
#include "../../inc/visualization/shader_loader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

typedef struct {
	GLfloat x, y, z;
	GLfloat nx, ny, nz;
} Vertex;

//Just to not have to type this behemoth in main draw_mesh
#define NODE2VERTEX(n) ( (Vertex) { (GLfloat)n.get_pos()[0], \
									(GLfloat)n.get_pos()[1], \
									(GLfloat)n.get_pos()[2], \
									(GLfloat)n.get_normal()[0], \
									(GLfloat)n.get_normal()[1], \
									(GLfloat)n.get_normal()[2] } )

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

	//load shader program
	GLuint shader_id = ShaderLoader::load("./shaders/flat");

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

	//load data
	GLuint vertex_array_id;
	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);

	GLuint vertex_buffer_id;
	glGenBuffers(1, &vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

	glBufferData(GL_ARRAY_BUFFER,
				vertice.size()*sizeof(Vertex),
				&vertice[0],
				GL_STATIC_DRAW);

	GLuint pos_id = glGetAttribLocation(shader_id, "pos");
	glEnableVertexAttribArray(pos_id);
	glVertexAttribPointer(pos_id,
							3,				//3 floats per Pos element
							GL_FLOAT,		//each element is a float
							GL_FALSE,		//do normalize values
							sizeof(Vertex), //displacement to next element
							0);				//offset inside sizeof(Vertex) range

	GLuint normal_id = glGetAttribLocation(shader_id, "normal");
	glEnableVertexAttribArray(normal_id);
	glVertexAttribPointer(normal_id,
							3,
							GL_FLOAT,
							GL_TRUE,
							sizeof(Vertex),
							(GLvoid*) (3*sizeof(GL_FLOAT)) );


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//load uniforms
	glm::mat4 view = glm::lookAt( glm::vec3(0.0f, 0.0f, 30.0f), 
									glm::vec3(15.0f, 20.0f, 0.0f),
									glm::vec3(0.0f, 1.0f, 0.0f) );

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.5f, 30.0f);
	glm::mat4 vp = projection * view;

	GLuint vp_id = glGetUniformLocation(shader_id, "vp");
	
	//main loop
	do
	{
		//Clear screen -> this function also clears stencil and depth buffer
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glUseProgram(shader_id);

		glUniformMatrix4fv(vp_id, 1, GL_FALSE, &vp[0][0]);

		glBindVertexArray(vertex_buffer_id);
		glDrawArrays(GL_TRIANGLES, 0, vertice.size());
		glBindVertexArray(0);

		glUseProgram(0);

		//Swap buffer and query events
		glfwSwapBuffers(this->window);
		glfwPollEvents();

	} while(glfwGetKey(this->window, GLFW_KEY_ESCAPE) != GLFW_PRESS && 
			!glfwWindowShouldClose(this->window));

	//clean everything
	this->terminate_rendering();
}
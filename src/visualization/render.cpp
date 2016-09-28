#include "../../inc/visualization/render.h"
#include "../../inc/visualization/shader_loader.h"
#include "../../inc/math/linalg.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>

#define ROTATION_SPEED 0.008

struct _vertex {
	glm::vec3 pos, normal, color;
};

//Just to not have to type this behemoth in main pack_geometry_data
#define NODE2VERTEX(n) ( (Vertex){n.get_pos(), n.get_normal(), n.get_color()} )

//----------------------------------
//----------- Internal -------------
//----------------------------------
static void pack_geometry_data(const Graph& in, std::vector<Vertex>& out)
{
	//pack mesh data into vertex buffer
	for(int i = 0; i < in.n_faces(); i++)
	{
		const Face& f = in.get_face(i);

		//pack data
		const Node &f1 = in.get_node(f.a), 
					&f2 = in.get_node(f.b), 
					&f3 = in.get_node(f.c);

		out.push_back( NODE2VERTEX(f1) );
		out.push_back( NODE2VERTEX(f2) );
		out.push_back( NODE2VERTEX(f3) );
	}
}

static double get_max_coord(const std::vector<Vertex>& mesh)
{
	double max = std::numeric_limits<double>::lowest();

	std::vector<Vertex>::const_iterator n = mesh.begin();
	for( ; n != mesh.end(); ++n)
		for(int i = 0; i < 3; i++)
		{
			double cur = glm::abs( n->pos[i] );
			max = cur > max ? cur : max;
		}

	return max;
}

static void compute_viewprojection(const std::vector<Vertex>& mesh, glm::mat4& vp, glm::dvec3& centroid_out)
{
	//compute translation which sends the centroid of the mesh to the origin
	std::vector<glm::dvec3> points;
	for( std::vector<Vertex>::const_iterator p = mesh.begin(); p != mesh.end(); ++p )
		points.push_back( p->pos );

 	glm::dvec3 centroid = centroid_out = cloud_centroid(points);
 	glm::dmat4 to_origin = glm::translate(glm::dmat4(1.0), -centroid);

 	//compute scale which fits the mesh inside a box with side equals to 2 units
 	double extreme = get_max_coord(mesh);
 	glm::dmat4 to_unit = glm::scale(glm::dmat4(1.0), glm::dvec3(1.0/extreme) );

 	//final transformation
 	glm::dmat4 T = to_unit * to_origin;

 	//Placing a camera at (0,0,1.5), looking at (0,0,0) and with FOV 45° gives
 	//us a nice picture of a box with side 2 placed at the origin. We build this
 	//camera, then we transform it to the space of our original mesh by multiplying
 	//it by T-¹.
 	glm::dmat4 inv_T = glm::inverse(T);
 	glm::dvec3 cam_pos = glm::dvec3( inv_T * glm::dvec4(0, 0, 1.5, 1.0) );
 	glm::dvec3 look_at = glm::dvec3( inv_T * glm::dvec4(0, 0, 0, 1.0) );

 	//build final view-projection matrix (that 3 came out from nowhere and I hope it will never
 	//crash. This "guarantees" the molecule will be within our field of vision -> remember we 
 	//multiplied extreme by 1.5 in the scaling operation).
 	glm::dmat4 view = glm::lookAt(cam_pos, look_at, glm::dvec3(0.0,1.0,0.0));
 	glm::dmat4 projection = glm::perspective(glm::radians(45.0), 4.0/3.0, 0.5, 3.0*extreme);

 	vp = static_cast<glm::mat4>( projection * view );
}

//---------------------------------------
//----------- From render.h -------------
//---------------------------------------
Render* Render::instance_ptr = 0;

Render::Render() { }

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

void Render::draw_meshes(const Graph& mesh1, const Graph& mesh2)
{
	// Merge both meshes into a single one
	std::vector<Vertex> mesh_data;

	pack_geometry_data(mesh1, mesh_data);
	pack_geometry_data(mesh2, mesh_data);

	this->draw_geometry_data(mesh_data);
}

void Render::draw_mesh(Graph& mesh)
{
	std::vector<Vertex> mesh_data;

	pack_geometry_data(mesh, mesh_data);

	this->draw_geometry_data(mesh_data);	
}

void Render::draw_geometry_data(const std::vector<Vertex>& geometry_data)
{
	this->setup_window();

	//load shader program
	GLuint shader_id = ShaderLoader::load("./shaders/flat");

	//load mesh data
	GLuint vertex_array_id;
	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);

	GLuint vertex_buffer_id;
	glGenBuffers(1, &vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

	glBufferData(GL_ARRAY_BUFFER,
				geometry_data.size()*sizeof(Vertex),
				&geometry_data[0],
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

	GLuint color_id = glGetAttribLocation(shader_id, "color");
	glEnableVertexAttribArray(color_id);
	glVertexAttribPointer(color_id,
							3,
							GL_FLOAT,
							GL_TRUE,
							sizeof(Vertex),
							(GLvoid*) (6*sizeof(GL_FLOAT)) );


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//compute uniform data
	glm::dvec3 centroid;

	glm::mat4 vp; compute_viewprojection(geometry_data, vp, centroid);
	GLuint vp_id = glGetUniformLocation(shader_id, "vp");

	//dumb animation
	glm::mat4 t1, R, t2, model_mat; float angle = 0.0f;
	GLuint model_id = glGetUniformLocation(shader_id, "model");
	t1 = glm::translate(glm::mat4(1.0f), static_cast<glm::vec3>(-centroid));
	t2 = glm::translate(glm::mat4(1.0f), static_cast<glm::vec3>(centroid));

	//main loop
	do
	{
		//Clear screen -> this function also clears stencil and depth buffer
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		//compute dumb animation rotation matrix
		R = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
		model_mat = t2 * R * t1; angle += ROTATION_SPEED;

		//load parameters and draw stuff
		glUseProgram(shader_id);

		glUniformMatrix4fv(vp_id, 1, GL_FALSE, &vp[0][0]);
		glUniformMatrix4fv(model_id, 1, GL_FALSE, &model_mat[0][0]);

		glBindVertexArray(vertex_buffer_id);
		glDrawArrays(GL_TRIANGLES, 0, geometry_data.size());
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
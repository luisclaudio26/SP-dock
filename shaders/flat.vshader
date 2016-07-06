#version 150

struct _model {
	//sends point from model to world space
	mat4 transform;

	//Intensity and color for diffuse, 
	//ambient and specular components
	float kD; vec3 cD;
	float kA; vec3 cA;
};
//uniform _model model;

//View-projection matrix
uniform mat4 vp;

in vec3 pos;
in vec3 normal;

flat out vec3 vcolor;

void main() 
{
	//Apply Model-View-Projection matrix to vertice
	gl_Position = vp * vec4(pos, 1.0);

	vcolor = vec3(1.0f, 0.0f, 0.0f);
}
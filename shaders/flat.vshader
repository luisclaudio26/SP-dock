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

	//light direction
	vec3 light_dir = normalize( vec3(-1.0f, -1.0f, -1.0f) );

	float spec_power = dot(-light_dir, normal);
	spec_power = max(0.0, spec_power);

	vcolor = vec3(0.0f, 0.2f, 0.0f) + vec3(1.0f,0.0f,0.0f)*spec_power;
}
#version 150

//Model matrix
uniform mat4 model;

//View-projection matrix
uniform mat4 vp;

in vec3 pos;
in vec3 normal;
in vec3 color;

flat out vec3 vcolor;

void main() 
{
	//Apply Model-View-Projection matrix to vertice
	gl_Position = vp * model * vec4(pos, 1.0);

	//light direction
	vec3 light_dir = normalize( vec3(-1.0f, -1.0f, -1.0f) );

	float spec_power = dot(-light_dir, normal);
	spec_power = max(0.0, spec_power);

	vcolor = color*(0.1f + spec_power);
}
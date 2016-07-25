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
	gl_Position = vp * model * vec4(pos, 1.0f);

	//rotate normals
	vec3 r_normal = vec3( vp * model * vec4(normal, 0.0f) );

	//light direction
	vec3 light_dir = normalize( vec3(0.0f, 0.0f, 1.0f) );

	float spec_power = dot(-light_dir, r_normal);
	spec_power = max(0.0, spec_power);

	vcolor = color*(0.1f + spec_power);
}
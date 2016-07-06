#version 150

struct _model {
	mat4 transform;

	//Intensity and color for diffuse, 
	//ambient and specular components
	float kD; vec3 cD;
	float kA; vec3 cA;
	float kS; vec3 cS;
	float shininess;
};
uniform _model model;


const int N_LIGHTS = 2;
struct _light {
	vec3 pos;
	float intensity;
	float falloff;
};
uniform _light light[N_LIGHTS];


//View-projection matrix and camera position
uniform mat4 vp;
uniform vec3 eye;

in vec3 pos;
in vec3 normal;
in vec3 tex;

flat out vec3 vcolor;

void main() 
{
	//Apply Model-View-Projection matrix to vertice
	gl_Position = vp * model.transform * vec4(pos, 1.0);

	//Common values
	float diffuse_acc = 0.0f, specular_acc = 0.0f;

	for(int i = 0; i < N_LIGHTS; i++)
	{
		vec3 pos2light = light[i].pos - pos;
		float distance2 = dot(pos2light,pos2light);
		float lightFalloff = distance2 * light[i].falloff;

		//------- Compute diffuse component --------
		float diffuse = max(0.0, dot(normalize(pos2light), normal) );
		diffuse_acc += (diffuse * light[i].intensity * model.kD) / lightFalloff;

		//------- Compute specular component --------
		vec3 reflected = normalize( reflect(-pos2light, normal) );
		vec3 pos2eye = normalize( eye - pos );
		float spec = dot(reflected, pos2eye);

		//If specular factor is less then zero (i.e., we're not looking directly to
		//the surface), clamp to zero. If not, apply shininess factor
		if(spec <= 0.0f) spec = 0.0f;
		else spec = pow(spec, model.shininess);

		specular_acc += (model.kS * spec * light[i].intensity) / lightFalloff;
	}

	vcolor = model.kA * model.cA;
	vcolor += diffuse_acc * model.cD;
	vcolor += specular_acc * model.cS;
}
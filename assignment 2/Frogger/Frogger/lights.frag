#version 150 core

out vec4 outFrag;

struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	int texCount;
};
uniform Materials mat;

in Data {
	float stateGlobal;
	float stateLamp;
	vec3 normal;
	vec3 lightDir;
	//vec3 point[6];
	vec3 eye;
} DataIn;

void main(void)
{
	vec4 dirLight = vec4(0.0); 

	vec4 spec = vec4(0.0);
	vec4 l_dif = vec4(0.0);

	vec3 n = vec3(0.0);
	vec3 l = vec3(0.0);
	vec3 e = vec3(0.0);
	vec3 h = vec3(0.0);

	float intensity = 0.0;
	float intSpec = 0.0;

	if(DataIn.stateGlobal == 1.0){

		n = normalize(DataIn.normal);
		l = normalize(DataIn.lightDir);
		e = normalize(DataIn.eye);

		intensity = max(dot(n,l), 0.0);
	
		if (intensity > 0.0) {
			l_dif = intensity * mat.diffuse;

			h = normalize(l + e);
			intSpec = max(dot(h,n), 0.0);
			spec = mat.specular * pow(intSpec, mat.shininess);
		}

		dirLight = spec + l_dif; 
	}

	outFrag = dirLight, mat.ambient;
}
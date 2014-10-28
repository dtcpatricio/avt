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
	vec3 lamps[6];
	vec3 eye;
} DataIn;

void main(void)
{
	vec4 dirLight = vec4(0.0); 
	vec4 pointLight = vec4(0.0);

	vec3 n = vec3(0.0);
	//vec3 l = vec3(0.0);
	vec3 e = vec3(0.0);
	//vec3 h = vec3(0.0);

	// Constants for attenuation
	float a = .1;
	float b = .2;
	float c = 0.3;

	n = normalize(DataIn.normal);
	e = normalize(DataIn.eye);


	////////////////////////////////////////
	// Directional Light

	vec3 l_dir = vec3(0.0);
	vec3 h_dir = vec3(0.0);
	vec4 spec_dir = vec4(0.0);
	vec4 l_dif_dir = vec4(0.0);
	float intensity_dir = 0.0;
	float intSpec_dir = 0.0;
	if(DataIn.stateGlobal == 1.0){

		l_dir = normalize(DataIn.lightDir);		

		intensity_dir = max(dot(n,l_dir), 0.0);
	
		if (intensity_dir > 0.0) {
			l_dif_dir = intensity_dir * mat.diffuse;

			h_dir = normalize(l_dir + e);
			intSpec_dir = max(dot(h_dir,n), 0.0);
			spec_dir = mat.specular * pow(intSpec_dir, mat.shininess);
		}

		dirLight = spec_dir + l_dif_dir; 
	}

	///////////////////////////////////////
	// Point Light
	
	vec3 l_pt = vec3(0.0);
	vec3 h_pt = vec3(0.0);
	vec4 spec_pt = vec4(0.0);
	vec4 l_dif_pt = vec4(0.0);
	float intensity_pt = 0.0;
	float intSpec_pt = 0.0;
	float d = 0.0;

	if(DataIn.stateLamp == 1.0) {
		
		for(int i = 0; i < 6; i++) {
			d = length(DataIn.lamps[i]);
			l_pt = normalize(DataIn.lamps[i]);
			intensity_pt = max(dot(n, l_pt), 0.0);
			
			if(intensity_pt > 0.0) {
				l_dif_pt = intensity_pt * mat.diffuse;
			
				h_pt = normalize(l_pt + e);
			
				intSpec_pt = max(dot(h_pt, n), 0.0);
				spec_pt = mat.specular * pow(intSpec_pt, mat.shininess);
			
				float bd = b * d;
				pointLight += (l_dif_pt + spec_pt) / (a + bd + c * pow(d,2));
			}
		}
	}
	/*
	if(pointLight == vec4(0.0))
	{
		pointLight = vec4(1.0);
	}*/

	outFrag = max(dirLight + pointLight, mat.ambient);
}
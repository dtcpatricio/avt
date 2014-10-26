#version 150 core

out vec4 colorOut;

struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	int texCount;
};
uniform Materials mat;

struct LightSource {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	vec4 spotDir;
	float shininess;
	float cutoff;
};
uniform LightSource light;

in Data {
	vec3 normal;
	vec3 eye;
	vec3 lightDir;
} DataIn;

void main(void) {

	float intensity = 0.0;
	vec4 spec = vec4(0.0);

	vec3 ld = normalize(DataIn.lightDir);
	vec3 sd = normalize(vec3(-(light.spotDir)));	

	// inside the cone?
	if (dot(sd,ld) > light.cutoff) {
		
		vec3 n = normalize(DataIn.normal);
		intensity = max(dot(n,ld), 0.0);
		
		if (intensity > 0.0) {
			vec3 eye = normalize(DataIn.eye);
			vec3 h = normalize(ld + eye);
			float intSpec = max(dot(h,n), 0.0);
			spec = mat.specular * pow(intSpec, mat.shininess);
		}
	}
	
	colorOut = max(intensity * mat.diffuse + spec, mat.ambient);
}
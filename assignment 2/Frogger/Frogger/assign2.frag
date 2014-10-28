#version 150 core

uniform int tex_needed;
uniform sampler2D texmap_road, texmap_river;

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

struct LightSource {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
};
uniform LightSource light;

in Data {
	vec3 normal;
	vec3 eye;
	vec3 lightDir;
    vec2 texCoordV;
} DataIn;

void main(void)
{
	vec4 spec = vec4(0.0);
	vec4 l_dif = vec4(0.0);

	vec3 n = normalize(DataIn.normal);
	vec3 l = normalize(DataIn.lightDir);
	vec3 e = normalize(DataIn.eye);

	float intensity = max(dot(n,l), 0.0);
	
	if (intensity > 0.0) {
		l_dif = intensity * light.diffuse * mat.diffuse;

		vec3 h = normalize(l + e);
		float intSpec = max(dot(h,n), 0.0);
		spec = light.specular * mat.specular * pow(intSpec, mat.shininess);
	}

	outFrag = max(mat.ambient * light.ambient, spec + l_dif);
}
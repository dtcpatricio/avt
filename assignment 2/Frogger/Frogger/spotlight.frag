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
	vec4 spotDir;
	float shininess;
	float cutoff;
};
uniform LightSource light;

in Data {
	vec3 normal;
	vec3 eye;
	vec3 lightDir;
    vec2 texCoordV;
} DataIn;

void main(void) {

	float intensity = 0.0;
	vec4 spec = vec4(0.0);

	vec3 ld = normalize(DataIn.lightDir);
	vec3 sd = normalize(vec3(-(light.spotDir)));
	
	bool inside_cone = false;	

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

		inside_cone = true;
	}
	
	// Creative way of applying ambient component
	vec4 realAmbient = 2.f * (mat.ambient * light.ambient);

	// outFrag = max(intensity * mat.diffuse + spec, realAmbient);
	vec4 matOutFrag = max(intensity * mat.diffuse + spec, realAmbient);

	if ((tex_needed == -1) || (inside_cone == false)) {
		outFrag = matOutFrag;
	}
	else if (tex_needed == 0) {
		vec4 texel = texture(texmap_river, DataIn.texCoordV);
		outFrag = (0.5 * matOutFrag) + (0.5 * texel);
	}
	else if (tex_needed == 1) {
		vec4 texel = texture(texmap_road, DataIn.texCoordV);
		outFrag = (0.5 * matOutFrag) + (0.5 * texel);
	}
}
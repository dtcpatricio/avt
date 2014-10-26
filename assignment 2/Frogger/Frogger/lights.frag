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

struct LightSource {
	vec4 pos;
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
};

uniform LightSource light;

struct Attenuation {
	float Constant;
	float Linear;
	float Exp;
};

struct Point{
	vec4 pos;
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	Attenuation a;
};


uniform Point pointLights[6];


in Data {
	vec3 normal;
	vec3 eye;
} DataIn;

vec4 calcLight(vec3 lightDir, vec4 ambient, vec4 diffuse, vec4 specular){
	vec4 spec = vec4(0.0);
	vec4 l_dif = vec4(0.0);
	vec4 res = vec4(0.0f);

	vec3 n = normalize(DataIn.normal);
	vec3 l = normalize(lightDir);
	vec3 e = normalize(DataIn.eye);

	float intensity = max(dot(n,l), 0.0);
	
	if (intensity > 0.0) {
		l_dif = intensity * diffuse * mat.diffuse;

		vec3 h = normalize(l + e);
		float intSpec = max(dot(h,n), 0.0);
		spec = specular * mat.specular * pow(intSpec, mat.shininess);
	}

	res = max(mat.ambient * light.ambient, spec + l_dif);
	return res;
}

vec4 calcDirLight(){
	
	return calcLight(light.pos.xyz, light.ambient, light.diffuse, light.specular);
}

vec4 calcPointLight(Point l){
	vec3 lightDir = vec3(0.0f);
	vec4 Color = vec4(0.0f);
	
	lightDir = l.pos.xyz + DataIn.eye;
	
	float distance = length(lightDir);
	
	Color = calcLight(lightDir, l.ambient, l.diffuse, l.specular);
	
	float att = l.a.Constant + 
				l.a.Linear * distance + 
				l.a.Exp * pow(distance, 2);

	return Color/att;
}

void main(void)
{
	vec4 TotalLight = vec4(0.0f);

	TotalLight = calcDirLight();

	for(int i = 0; i < 6; i ++){
		TotalLight += calcPointLight(pointLights[i]);
	}                                 

	outFrag = TotalLight;
}
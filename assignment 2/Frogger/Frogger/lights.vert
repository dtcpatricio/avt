#version 150 core

uniform mat4 projMatrix, viewMatrix, model;
uniform mat3 m_normal;

uniform vec4 l_pos;

uniform vec4 point[6];

uniform float stateGbl;
uniform float stateL;

in vec4 in_pos;
in vec4 normal;

out Data {
	float stateGlobal;
	float stateLamp;
	vec3 normal;
	vec3 lightDir;
	//vec3 point[6];
	vec3 eye;
} DataOut;

void main(void)
{
	vec4 pos = viewMatrix * model * in_pos;

	DataOut.stateGlobal = stateGbl;
	DataOut.stateLamp = stateL;
	DataOut.normal = normalize(m_normal * normal.xyz);
	DataOut.lightDir = vec3(l_pos);

	/*for(int i = 0; i<6; i++){
		vec3 dir =  point[i].xyz - pos.xyz;
		DataOut.point[i] = dir;
	}*/

	DataOut.eye = vec3(-pos);

	gl_Position = projMatrix * viewMatrix * model * in_pos;
}

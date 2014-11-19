#version 330

uniform mat4 projMatrix, viewMatrix, model, projFlare;
uniform mat3 m_normal;

uniform float billboard; 

uniform float flareBool;

uniform vec4 l_pos;

uniform vec4 lamp1, lamp2, lamp3, lamp4, lamp5, lamp6;

uniform float stateGbl;
uniform float stateL;

in vec4 in_pos;
in vec4 normal;
in vec4 texCoord;

out Data {
	float stateGlobal;
	float stateLamp;
	vec3 normal;
	vec3 lightDir;
	vec3 lamps[6];
	vec3 eye;
	vec2 tex_coord;
	int color;
} DataOut;

void main(void)
{
	mat4 projectionM = projMatrix;
	if(flareBool == 1.0) {
		projectionM = projFlare;

	}
	mat4 viewModel = viewMatrix * model;

	if(billboard == 1.0) {
		viewModel[0].xyz = vec3(1.0, 0.0, 0.0);
		viewModel[1].xyz = vec3(0.0, 1.0, 0.0);
		viewModel[2].xyz = vec3(0.0, 0.0, 1.0);
	}

	vec4 pos = viewModel * in_pos;

	DataOut.stateGlobal = stateGbl;
	DataOut.stateLamp = stateL;
	DataOut.normal = normalize(m_normal * normal.xyz);
	DataOut.lightDir = vec3(l_pos);

	DataOut.lamps[0]= lamp1.xyz - pos.xyz; 
	DataOut.lamps[1]= lamp2.xyz - pos.xyz;
	DataOut.lamps[2]= lamp3.xyz - pos.xyz;
	DataOut.lamps[3]= lamp4.xyz - pos.xyz;
	DataOut.lamps[4]= lamp5.xyz - pos.xyz;
	DataOut.lamps[5]= lamp6.xyz - pos.xyz;
	
	DataOut.eye = vec3(-pos);
	DataOut.tex_coord = texCoord.st;
	gl_Position = projectionM * viewModel * in_pos;
}
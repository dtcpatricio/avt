#version 150 core

uniform mat4 projMatrix, viewMatrix, model;
uniform mat3 m_normal;

uniform vec4 l_pos;

uniform vec4 lamp1;
uniform vec4 lamp2;
uniform vec4 lamp3;
uniform vec4 lamp4;
uniform vec4 lamp5;
uniform vec4 lamp6;

uniform float stateGbl;
uniform float stateL;

in vec4 in_pos;
in vec4 normal;

out Data {
	float stateGlobal;
	float stateLamp;
	vec3 normal;
	vec3 lightDir;
	vec3 lamps[6];
	vec3 eye;
} DataOut;

void main(void)
{
	vec4 pos = viewMatrix * model * in_pos;

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

	gl_Position = projMatrix * viewMatrix * model * in_pos;
}
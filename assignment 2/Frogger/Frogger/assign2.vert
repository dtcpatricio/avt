#version 330 core

uniform mat4 projMatrix, viewMatrix, model;
uniform mat3 m_normal;

uniform vec4 l_pos;

in vec4 in_pos;
in vec3 normal;

out Data {
	vec3 normal;
	vec3 eye;
	vec3 lightDir;
} DataOut;

void main(void)
{
	vec4 pos = viewMatrix * model * in_pos;

	DataOut.normal = normalize(m_normal * normal);
	DataOut.lightDir = vec3(l_pos.xyz);
	DataOut.eye = vec3(-pos);

	gl_Position = projMatrix * viewMatrix * model * in_pos;
}

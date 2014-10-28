#version 150 core

uniform mat4 projMatrix, viewMatrix, model;
uniform mat3 m_normal;

uniform vec4 l_pos, l_spotDir;

in vec4 in_pos;
in vec3 normal;
in vec2 texCoord;

out Data {
	vec3 normal;
	vec3 eye;
	vec3 lightDir;
    vec2 texCoordV;
} DataOut;

void main(void)
{
	DataOut.texCoordV = texCoord;
	vec4 pos = viewMatrix * model * in_pos;

	DataOut.normal = normalize(m_normal * normal);
	DataOut.lightDir = vec3(l_pos - pos);
	DataOut.eye = vec3(-pos);

	gl_Position = projMatrix * viewMatrix * model * in_pos;
}

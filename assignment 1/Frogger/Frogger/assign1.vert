#version 150 core

in vec4 in_pos;
uniform mat4 projMatrix, viewMatrix;
out vec4 color;

void main(void)
{
	color = in_pos;
	gl_Position = projMatrix * viewMatrix * in_pos;
}
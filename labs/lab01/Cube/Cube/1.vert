#version 150 core

in vec4 in_Position;
uniform mat4 projMatrix, viewMatrix;
out vec4 color;

void main(void)
{
	color = in_Position;
	gl_Position = projMatrix * viewMatrix * in_Position;
}
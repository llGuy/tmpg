#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in mat4 model_matrix;

uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main(void)
{
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0f);
}
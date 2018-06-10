#version 330 core

layout(location = 0) in vec3 vertex_position;
out vec3 pass_world_position;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main(void)
{
	vec4 world_position = model_matrix * vec4(vertex_position, 1.0f);
	pass_world_position = vec3(world_position.xyz);

	vec4 view_position = view_matrix * world_position;
	gl_Position = projection_matrix * view_position;
}
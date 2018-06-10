#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;
//out vec4 diffuse;

in vec3 pass_world_position[];

uniform vec3 light_position;

/*vec3 Normal(void)
{
	vec3 diff_world_pos1 = normalize(pass_world_position[1] - pass_world_position[0]);
	vec3 diff_world_pos2 = normalize(pass_world_position[2] - pass_world_position[0]);
	return normalize(cross(diff_world_pos2, diff_world_pos1));
}

vec4 Diffuse(vec3 normal, vec3 light_vector)
{
	float brightness = dot(light_vector, normalize(normal)) * 0.8f;
	return vec4(brightness, brightness, brightness, 1.0);
}
*/
void main(void)
{
	//vec3 light_vector = normalize(light_position - pass_world_position[0]);

	//vec3 normal = Normal();
	//vec4 d = Diffuse(normal, light_vector);

	for (int i = 0; i < 3; ++i)
	{
		gl_Position = gl_in[i].gl_Position;
		//diffuse = d;

		EmitVertex();
	}
	EndPrimitive();
}
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoords;

out VS_OUT {
	vec3 pass_pos;
	vec3 pass_nor;
	vec2 pass_tcoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main(){
	vs_out.pass_pos = position;
	vs_out.pass_nor = normal;
	vs_out.pass_tcoords = texcoords;
	gl_Position = projection * view * vec4(position, 1.0);
}
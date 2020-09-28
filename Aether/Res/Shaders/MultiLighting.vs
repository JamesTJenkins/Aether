#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec3 pass_pos;
out vec3 pass_normal;

void main(){
	pass_pos = vec3(transform * vec4(Position, 1.0));
	pass_normal = mat3(transpose(inverse(transform))) * Normal;
	gl_Position = projection * view * vec4 (pass_pos, 1.0);
}
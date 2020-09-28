#version 400

in vec3 position;
in vec2 uv;
in vec3 normal;

uniform mat4 transform;

out vec3 pass_position;
out vec3 pass_normal;

void main(){
	pass_normal = normal;
	pass_position = position;
	gl_Position = transform * vec4 (position, 1.0);
}
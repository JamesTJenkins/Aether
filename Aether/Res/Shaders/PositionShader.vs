#version 400

in vec3 position;
in vec2 uv;
uniform mat4 transform;

out vec2 pass_uv;

void main(){
	gl_Position = transform * vec4 (position, 1.0);
	pass_uv = uv;
}
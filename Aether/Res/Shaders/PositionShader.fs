#version 400

in vec2 pass_uv;
out vec4 out_color;

void main(){
	out_color = vec4(pass_uv, 0.0, 1.0);
}
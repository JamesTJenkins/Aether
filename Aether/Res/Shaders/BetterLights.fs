#version 330 core

in VS_OUT {
	vec3 pass_pos;
	vec3 pass_nor;
	vec2 pass_tcoords;
} fs_in;

uniform vec3 color;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 out_color;

void main(){
	// Color here if texture exists
	
	// Ambient
	vec3 ambient = 0.05 * color;
	// Diffuse
	vec3 lightDir = normalize(lightPos - fs_in.pass_pos);
	vec3 normal = normalize(fs_in.pass_nor);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = color * diff;
	// Spec
	vec3 viewDir = normalize(viewPos - fs_in.pass_pos);
	vec3 reflectDir = reflect(-lightDir, normal);
	// Blinn
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
	// Out
	vec3 specular = vec3(0.3) * spec;
	out_color = vec4(ambient + diffuse + specular, 1.0);
}
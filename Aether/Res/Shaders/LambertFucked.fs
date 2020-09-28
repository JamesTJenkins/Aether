#version 400

in vec3 pass_position;
in vec3 pass_normal;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightDir;
uniform vec3 lightColor;

out vec4 out_color;

void main(){
	// Ambient color
	vec3 ambient = lightColor * material.ambient;

	// Diffuse
	vec3 lightDir = normalize(lightPos - pass_position);
	float diff = max(0.0, dot(pass_normal, lightDir));
	vec3 diffuse = diff * lightColor;

	// Specular
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - pass_position);
	vec3 reflectDir = reflect(-lightDir, pass_normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * material.diffuse;
	out_color = vec4 (result, 1.0);
}
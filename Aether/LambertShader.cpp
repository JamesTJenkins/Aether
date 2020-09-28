#include "LambertShader.h"

namespace Shaders {

	LambertShader::LambertShader(const std::string& filename) : Shader(filename, Shader::VERTEX | Shader::FRAGMENT) {
		glBindAttribLocation(program, POSITION_A, "position");
		glBindAttribLocation(program, TEXCOORDS_A, "uv");
		glBindAttribLocation(program, NORMALS_A, "normal");

		uniforms[TRANSFORM_UNIFORM] = glGetUniformLocation(program, "transform");
		uniforms[LIGHTDIR_UNIFORM] = glGetUniformLocation(program, "lightDir");
	}

	void LambertShader::Update(const Camera& camera, const Material& material, const glm::vec3& viewPos, const glm::vec3& lightPos, const glm::vec3& lightDir, const glm::vec3& lightColor) {
		glUniformMatrix4fv(uniforms[TRANSFORM_UNIFORM], 1, GL_FALSE, &camera.WorldToCameraMatrix()[0][0]);
		glUniform3fv(uniforms[VIEWPOS_UNIFORM], 1, (GLfloat*)&viewPos);
		glUniform3fv(uniforms[LIGHTPOS_UNIFORM], 1, (GLfloat*)&lightPos);
		glUniform3fv(uniforms[LIGHTDIR_UNIFORM], 1, (GLfloat*)&lightDir);
		glUniform3fv(uniforms[LIGHTCOLOR_UNIFORM], 1, (GLfloat*)&lightColor);

		SetVec3("material.ambient", material.ambientColor);
		SetVec3("material.diffuse", material.diffuseColor);
		SetVec3("material.specular", material.specularColor);
		SetFloat("material.shininess", material.specularExponent);
	}

}
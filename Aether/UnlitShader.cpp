#include "UnlitShader.h"

namespace Shaders {

	UnlitShader::UnlitShader(const std::string& filename) : Shader(filename, Shader::VERTEX | Shader::FRAGMENT) {
		glBindAttribLocation(program, POSITION_A, "position");
		glBindAttribLocation(program, TEXCOORDS_A, "uv");

		uniforms[TRANSFORM_UNIFORM] = glGetUniformLocation(program, "transform");
	}

	void UnlitShader::Update(const Camera& camera) {
		glUniformMatrix4fv(uniforms[TRANSFORM_UNIFORM], 1, GL_FALSE, &camera.WorldToCameraMatrix()[0][0]);
	}
}
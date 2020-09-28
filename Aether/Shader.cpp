#include "Shader.h"
#include <fstream>
#include <iostream>

namespace Shaders {

	Shader::Shader(const std::string& filename, unsigned int shaders) : shaders(shaders) {
		program = glCreateProgram();

		if (shaders & VERTEX) {
			shader[VERTEX_SHADER] = CreateShader(ReadShader(filename + ".vs"), GL_VERTEX_SHADER);
			glAttachShader(program, shader[VERTEX_SHADER]);
		}

		if (shaders & FRAGMENT) {
			shader[FRAGMENT_SHADER] = CreateShader(ReadShader(filename + ".fs"), GL_FRAGMENT_SHADER);
			glAttachShader(program, shader[FRAGMENT_SHADER]);
		}

		if (shaders & GEOMETRY) {
			shader[GEOMETRY_SHADER] = CreateShader(ReadShader(filename + ".gs"), GL_GEOMETRY_SHADER);
			glAttachShader(program, shader[GEOMETRY_SHADER]);
		}

		glLinkProgram(program);
		CheckShaderError(program, GL_LINK_STATUS, true, "Error, Program link error");

		glValidateProgram(program);
		CheckShaderError(program, GL_VALIDATE_STATUS, true, "Error, Program validation error");
	}

	Shader::~Shader() {
		if (shaders & VERTEX) {
			glDetachShader(program, shader[VERTEX_SHADER]);
			glDeleteShader(shader[VERTEX_SHADER]);
		}

		if (shaders & FRAGMENT) {
			glDetachShader(program, shader[FRAGMENT_SHADER]);
			glDeleteShader(shader[FRAGMENT_SHADER]);
		}

		if (shaders & GEOMETRY) {
			glDetachShader(program, shader[GEOMETRY_SHADER]);
			glDeleteShader(shader[GEOMETRY_SHADER]);
		}

		glDeleteProgram(program);
	}

	void Shader::Bind() {
		glUseProgram(program);
	}

	void Shader::SetBool(const std::string& valueName, bool value) const {
		glUniform1i(glGetUniformLocation(program, valueName.c_str()), (int)value);
	}

	void Shader::SetInt(const std::string& valueName, int value) const {
		glUniform1i(glGetUniformLocation(program, valueName.c_str()), value);
	}

	void Shader::SetFloat(const std::string& valueName, float value) const {
		glUniform1f(glGetUniformLocation(program, valueName.c_str()), value);
	}

	void Shader::SetVec3(const std::string& valueName, const glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation(program, valueName.c_str()), 1, &value[0]);
	}

	void Shader::SetVec3(const std::string& valueName, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(program, valueName.c_str()), x, y, z);
	}

	void Shader::SetMat4(const std::string& valueName, const glm::mat4& value) const {
		glUniformMatrix4fv(glGetUniformLocation(program, valueName.c_str()), 1, GL_FALSE, &value[0][0]);
	}

	std::string Shader::ReadShader(const std::string& filename) {
		std::ifstream file;
		file.open(filename.c_str());

		std::string line;
		std::string output;

		if (file.is_open()) {
			while (file.good()) {
				std::getline(file, line);
				output.append(line + '\n');
			}
		} else {
			std::cerr << "Error Reading Shader: " << filename << std::endl;
		}

		file.close();
		return output;
	}

	GLuint Shader::CreateShader(const std::string& text, GLenum shaderType) {
		GLuint shader = glCreateShader(shaderType);

		if (shader == 0) {
			std::cerr << "Error Creating Shader" << std::endl;
			return shader;
		}

		const GLchar* shaderSourceString;
		GLint shaderSourceStringLength;

		shaderSourceString = text.c_str();
		shaderSourceStringLength = (GLint)text.length();

		glShaderSource(shader, 1, &shaderSourceString, &shaderSourceStringLength);
		glCompileShader(shader);

		CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error, shader compilation error");

		return shader;
	}

	void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMsg) {
		GLint success = 0;
		GLchar error[1024] = { 0 };

		if (isProgram) {
			glGetProgramiv(shader, flag, &success);
		} else {
			glGetShaderiv(shader, flag, &success);
		}

		if (success == GL_FALSE) {
			if (isProgram) {
				glGetProgramInfoLog(shader, sizeof(error), nullptr, error);
			} else {
				glGetShaderInfoLog(shader, sizeof(error), nullptr, error);
			}

			std::cerr << errorMsg << ": '" << error << "'" << std::endl;
		}
	}
}
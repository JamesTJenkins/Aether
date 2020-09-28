#pragma once
#include <glm/glm.hpp>
#include <string>
#include <GL/glew.h>

namespace Shaders {

	class Shader {
	public:
		Shader(const std::string& filename, unsigned int shaders);
		virtual ~Shader();
		
		void Bind();

		void SetBool(const std::string& valueName, bool value) const;
		void SetInt(const std::string& valueName, int value) const;
		void SetFloat(const std::string& valueName, float value) const;
		void SetVec3(const std::string& valueName, const glm::vec3& value) const;
		void SetVec3(const std::string& valueName, float x, float y, float z) const;
		void SetMat4(const std::string& valueName, const glm::mat4& value) const;

		enum {
			VERTEX = 1u,
			FRAGMENT = 2u,
			GEOMETRY = 4u
		};

	protected:
		enum {
			VERTEX_SHADER,
			FRAGMENT_SHADER,
			GEOMETRY_SHADER,
			NUM_SHADERS
		};

		GLuint program;
		GLuint shader[NUM_SHADERS];

	private:
		static std::string ReadShader(const std::string& filename);
		static GLuint CreateShader(const std::string& text, GLenum shaderType);
		static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMsg);

		unsigned int shaders;
	};

}

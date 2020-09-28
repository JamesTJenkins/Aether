#pragma once
#include "Shader.h"
#include "Material.h"
#include "Camera.h"

namespace Shaders {

	class LambertShader : public Shader {
	public:
		LambertShader(const std::string& filename);
		void Update(const Camera& camera, const Material& material, const glm::vec3& viewPos, const glm::vec3& lightPos, const glm::vec3& lightDir, const glm::vec3& lightColor);

	private:
		enum {
			POSITION_A,
			TEXCOORDS_A,
			NORMALS_A,
			NUM_ATTRIBUTES
		};

		enum {
			TRANSFORM_UNIFORM,
			MATERIAL_UNIFORM,
			VIEWPOS_UNIFORM,
			LIGHTPOS_UNIFORM,
			LIGHTDIR_UNIFORM,
			LIGHTCOLOR_UNIFORM,
			NUM_UNIFORMS
		};

		GLuint uniforms[NUM_UNIFORMS];
	};

}
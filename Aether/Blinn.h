#pragma once
#include "Shader.h"
#include "Material.h"
#include "Camera.h"

namespace Shaders {

	class Blinn : public Shader {
	public:
		Blinn(const std::string& filename);
		void Update(const Camera& camera, const glm::vec3& color, const glm::vec3& lightPos);

	private:
		enum {
			POSITION_A,
			TEXCOORDS_A,
			NORMALS_A,
			NUM_ATTRIBUTES
		};

		enum {
			TRANSFORM_UNIFORM,
			NUM_UNIFORMS
		};

		GLuint uniforms[NUM_UNIFORMS];
	};

}


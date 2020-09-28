#pragma once
#include <string>
#include "Shader.h"
#include "Camera.h"
#include "Entity.fwd.h"
#include "Material.h"

namespace Shaders {

	class MultiLighting : public Shader {
	public:
		MultiLighting(const std::string& filename);
		void Update(const Camera& camera, const Material& material, Objects::GameObject* go);

	private:

		enum {
			POSITION_A,
			NORMAL_A,
			TEXCOORDS_A,
			NUM_ATTRIBUTES
		};

		enum {
			TRANSFORM_UNIFORM,
			NUM_UNIFORMS
		};

		GLuint uniforms[NUM_UNIFORMS];
	};

}

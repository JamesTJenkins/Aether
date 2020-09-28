#pragma once
#include "Shader.h"
#include "Camera.h"

namespace Shaders {

	class UnlitShader : public Shader {
	public:
		UnlitShader(const std::string& filename);
		void Update(const Camera& camera);

	private:
		enum {
			POSITION_A,
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
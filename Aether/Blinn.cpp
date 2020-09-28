#include "Blinn.h"

namespace Shaders {

	Blinn::Blinn(const std::string& filename) : Shader(filename, Shader::VERTEX | Shader::FRAGMENT) {
		glBindAttribLocation(program, POSITION_A, "position");
		glBindAttribLocation(program, TEXCOORDS_A, "texcoords");
		glBindAttribLocation(program, NORMALS_A, "normal");
	}

	void Blinn::Update(const Camera& camera, const glm::vec3& color, const glm::vec3& lightPos) {
		glm::mat4 view = camera.GetViewMatrix();
		SetMat4("projection", camera.GetProjectionMatrix());
		SetMat4("view", view);

		SetVec3("color", color);
		SetVec3("lightPos", lightPos);
		SetVec3("viewPos", camera.transform.position);
	}
}
#pragma once
#include<glm/glm.hpp>
#include<glm/gtx/quaternion.hpp>

namespace Objects {

	class Transform {
	public:
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;

		Transform(const glm::vec3& position = glm::vec3(0, 0, 0), const glm::quat& rotation = glm::quat(), const glm::vec3& scale = glm::vec3(1, 1, 1));

		glm::mat4 LocalToWorldMatrix() const;

		void Rotate(const glm::vec3& angles);
		void Rotate(float x, float y, float z);
		void RotateOnAxis(const float& angle, const glm::vec3& axis);
		void Quat2Euler(glm::quat quat);

		glm::vec3 Left() const;
		glm::vec3 Right() const;
		glm::vec3 Up() const;
		glm::vec3 Down() const;
		glm::vec3 Forward() const;
		glm::vec3 Back() const;
		glm::vec3 GetEulerAngles() const;

	private:
		glm::vec3 eulerAngles;
	};
}
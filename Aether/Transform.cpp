#include "Transform.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Objects {

	Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) : position(position), rotation(rotation), scale(scale) {
		eulerAngles = glm::eulerAngles(rotation) * 3.14159f / 180.f;
	}

	glm::mat4 Transform::LocalToWorldMatrix() const {
		glm::mat4 traslationMatrix = glm::translate(position);
		glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
		glm::mat4 scaleMatrix = glm::scale(scale);

		return traslationMatrix * rotationMatrix * scaleMatrix;
	}

	void Transform::Rotate(const glm::vec3& angles) {
		eulerAngles = angles;

		rotation =
			glm::angleAxis(glm::radians(angles.x), glm::vec3(1, 0, 0)) *
			glm::angleAxis(glm::radians(angles.y), glm::vec3(0, 1, 0)) *
			glm::angleAxis(glm::radians(angles.z), glm::vec3(0, 0, 1));
	}

	void Transform::Rotate(float x, float y, float z) {
		eulerAngles = glm::vec3(x, y, z);

		rotation =
			glm::angleAxis(glm::radians(x), glm::vec3(1, 0, 0)) *
			glm::angleAxis(glm::radians(y), glm::vec3(0, 1, 0)) *
			glm::angleAxis(glm::radians(z), glm::vec3(0, 0, 1));
	}

	void Transform::RotateOnAxis(const float& angle, const glm::vec3& axis) {
		rotation = glm::angleAxis(glm::radians(angle), axis);
	}

	void Transform::Quat2Euler(glm::quat quat) {
	}

	glm::vec3 Transform::Left() const {
		return rotation * glm::vec3(1, 0, 0);
	}

	glm::vec3 Transform::Right() const {
		return rotation * glm::vec3(-1, 0, 0);
	}

	glm::vec3 Transform::Up() const {
		return rotation * glm::vec3(0, 1, 0);
	}

	glm::vec3 Transform::Down() const {
		return rotation * glm::vec3(0, -1, 0);
	}

	glm::vec3 Transform::Forward() const {
		return rotation * glm::vec3(0, 0, 1);
	}

	glm::vec3 Transform::Back() const {
		return rotation * glm::vec3(0, 0, -1);
	}

	glm::vec3 Transform::GetEulerAngles() const {
		return eulerAngles;
	}
}

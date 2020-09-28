#include "Camera.h"
#include <glm/gtx/transform.hpp>

Camera::Camera(const Objects::Transform& transform, float aspect, float fov, float nearPlane, float farPlane) : transform (transform) {
	projectionMatrix = glm::perspective(fov, aspect, nearPlane, farPlane);
}

glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(transform.position, transform.position + transform.Forward(), transform.Up());
}

glm::mat4 Camera::WorldToCameraMatrix() const {
	return projectionMatrix * glm::lookAt(transform.position, transform.position + transform.Forward(), transform.Up());
}

glm::mat4 Camera::GetProjectionMatrix() const {
	return projectionMatrix;
}

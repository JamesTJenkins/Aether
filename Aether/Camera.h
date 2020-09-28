#pragma once
#include <glm/glm.hpp>
#include "Transform.h"

class Camera {
public:
	Camera(const Objects::Transform& transform, float aspect, float fov, float nearPlane, float farPlane);

	glm::mat4 GetViewMatrix() const;
	glm::mat4 WorldToCameraMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

	Objects::Transform transform;
private:
	glm::mat4 projectionMatrix;
};


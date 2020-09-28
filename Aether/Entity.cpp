#include "Entity.h"
#include <iostream>
using namespace std::placeholders;

namespace Objects {

	Entity::Entity(EngineSystems::EventHandler* eh, int id) : enitityId(id), eventh(eh) {

	}

	Entity::~Entity() {
		// Remove from logic loop
	}

	void Entity::Start() {

	}

	void Entity::Update() {

	}

	// TEST ENTITY

	TestEntity::TestEntity(EngineSystems::EventHandler* eh, int id) : Entity(eh, id) {

	}

	TestEntity::~TestEntity() {
		// Remove from logic loop
	}

	void TestEntity::Start() {
		EngineSystems::Message msg;
		msg.type = EngineSystems::Message::Logic_Event;
		msg.func = std::bind(&TestEntity::EventTest, this, std::placeholders::_1);
		msg.arguments = 0;

		eventh->RegisterEvent(msg);
	}

	void TestEntity::EventTest(int value) {
		std::cout << "WORKS" << std::endl;
	}

	// GAMEOBJECTS

	GameObject::GameObject(EngineSystems::EventHandler* eh, int id, const Transform& transform) : Entity(eh, id), transform(transform) {

	}

	GameObject::~GameObject() {

	}

	void GameObject::Start() {
		Shaders::Material mat;
		mat.diffuseColor = glm::vec3(0.0f, 1.0f, 0.0f);
		mat.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
		mat.specularExponent = 32.0f;

		mesh->meshMaterial = mat;
	}

	void GameObject::Update() {
		transform.position.x = glm::sin(f / 99.0) * 5;
		transform.position.y = glm::cos(f / 198.0) * 2;
		transform.Rotate(transform.GetEulerAngles().x + 5, 0, 0);
		++f;
	}

	void GameObject::Render(const Camera& camera) {
		mesh->shader->Update(camera, mesh->meshMaterial, this);
		mesh->Draw();
	}
}

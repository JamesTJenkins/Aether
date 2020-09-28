#pragma once
#include "Events.h"
#include "Transform.h"
#include "Mesh.h"

namespace Objects {

	class Entity {
	public:
		Entity(EngineSystems::EventHandler* eh, int id);
		~Entity();

		virtual void Start();
		virtual void Update();

		int enitityId;
		EngineSystems::EventHandler* eventh;
	};

	class GameObject : public Entity {
	public:
		GameObject(EngineSystems::EventHandler* eh, int id, const Transform& transform);
		~GameObject();

		void Start();
		void Update();
		void Render(const Camera& camera);

		Transform transform;
		Meshes::Mesh* mesh;

		int f = 0;
	};

	class TestEntity : public Entity {
	public:
		TestEntity(EngineSystems::EventHandler* eh, int id);
		~TestEntity();
		void Start();
		void EventTest(int value);
	};

}
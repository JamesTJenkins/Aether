#include "Logic.h"
#include <iostream>
#include "Mesh.h"
#include "MultiLighting.h"

namespace EngineSystems {

	Logic::Logic(EventHandler* eh) : eventh(eh) {

	}

	void Logic::UpdateLogic() {
		for (size_t i = 0; i < numEntities; i++) {
			gameEntities[i].Update();
		}
	}

	Objects::TestEntity* Logic::CreateTestEntity() {
		Objects::TestEntity* e = &Objects::TestEntity(eventh, numEntities);

		testEntities.push_back(e);
		numEntities++;

		return e;
	}

	void Logic::CreateGameEntity(Meshes::Mesh* mesh, std::shared_ptr<Shaders::MultiLighting> shader, Shaders::Material material, const Objects::Transform& transform) {
		Objects::GameObject e = Objects::GameObject(eventh, numEntities, Objects::Transform());
		e.mesh = mesh;
		e.mesh->shader = shader;
		e.mesh->meshMaterial = material;
		e.transform = transform;

		gameEntities.push_back(e);
		numEntities++;

		e.Start();
	}

	int Logic::GetEntityCount() {
		return numEntities;
	}
}
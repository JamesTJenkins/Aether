#pragma once
#include <vector>
#include "Entity.h"
#include "Events.h"
#include "Mesh.fwd.h"
#include "Shaders.fwd.h"

namespace EngineSystems {

	class Logic {
	public:
		Logic(EngineSystems::EventHandler* eh);

		void UpdateLogic();

		Objects::TestEntity* CreateTestEntity();
		void CreateGameEntity(Meshes::Mesh* mesh, std::shared_ptr<Shaders::MultiLighting> shader, Shaders::Material material, const Objects::Transform& transform);

		int GetEntityCount();

		std::vector<Objects::TestEntity*> testEntities;
		std::vector<Objects::GameObject> gameEntities;

	private:
		int maxEntities = 100;
		int numEntities = 0;
		EngineSystems::EventHandler* eventh;
	};

}
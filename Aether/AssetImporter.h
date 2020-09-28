#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "Mesh.h"
#include "Transform.h"
#include "Logic.h"

namespace Meshes {

	struct GameObjectNode {
	public:
		int id = 0;
		Objects::Transform transform;
		std::string meshPath;
		std::string shaderPath;
	};

	class AssetImporter {
	public:
		AssetImporter();
		~AssetImporter();
	
		// Meshes and Materials
	
		bool ImportObj(std::string path);
		bool LoadMaterials(std::string path);
		std::shared_ptr<Shaders::MultiLighting> LoadShader(std::string path);
	
		std::vector<Mesh*> loadedMeshes;
		std::vector<Vertex> loadedVertices;
		std::vector<unsigned int> loadedIndices;
		std::vector<Shaders::Material> loadedMaterials;
		std::unordered_map<std::string, std::shared_ptr<Shaders::MultiLighting>> loadedShaders;
	
		// Init List
	
		bool LoadInitList(std::string path, EngineSystems::Logic& logic);
	private:
		int loadedMeshesCount = 0;
	};

}
#pragma once
#include<glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <vector>
#include <memory>
#include "Material.h"
#include "MultiLighting.h"

namespace Meshes {

	struct Vertex {
	public:
		Vertex(const glm::vec3& pos = glm::vec3(), const glm::vec3& normal = glm::vec3(), const glm::vec2& texcoords = glm::vec2());

		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texcoords;
	};

	class Mesh {
	public:
		Mesh();
		Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
		~Mesh();

		void Draw();

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::string meshName;
		std::shared_ptr<Shaders::MultiLighting> shader;
		Shaders::Material meshMaterial;
	private:
		enum {
			VERTEX_VB,
			VERTEX_EB,
			NUM_BUFFER
		};

		enum {
			POSITION_ATTRIB,
			NORMAL_ATTRIB,
			TEXCOORDS_ATTRIB,
			NUM_ATTRIBUTES
		};

		GLuint vertexArrayObject;
		GLuint vertexArrayBuffers[NUM_BUFFER];
	};

}
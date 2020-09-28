#include "Mesh.h"
#include <GL/glew.h>

namespace Meshes {

	Vertex::Vertex(const glm::vec3& pos, const glm::vec3& normal, const glm::vec2& texcoords) : position(pos), normal(normal), texcoords(texcoords) {

	}

	Mesh::Mesh() {

	}

	Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) : vertices(vertices), indices(indices) {
		glGenVertexArrays(1, &vertexArrayObject);
		glGenBuffers(NUM_BUFFER, vertexArrayBuffers);

		glBindVertexArray(vertexArrayObject);

		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[VERTEX_VB]);
		glBufferData(GL_ARRAY_BUFFER, (int)(vertices.size()) * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(POSITION_ATTRIB);
		glVertexAttribPointer(POSITION_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(NORMAL_ATTRIB);
		glVertexAttribPointer(NORMAL_ATTRIB, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(TEXCOORDS_ATTRIB);
		glVertexAttribPointer(TEXCOORDS_ATTRIB, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoords));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[VERTEX_EB]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (int)(indices.size()) * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Mesh::~Mesh() {
		glDeleteVertexArrays(1, &vertexArrayObject);
		glDeleteBuffers(NUM_BUFFER, vertexArrayBuffers);
	}

	void Mesh::Draw() {
		glBindVertexArray(vertexArrayObject);

		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}

}

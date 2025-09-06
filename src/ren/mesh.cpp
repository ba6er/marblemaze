#include <ren/mesh.hpp>
#include <glad/glad.h>
#include <cstdlib>

using namespace ren;

Mesh::Mesh() : vao(0), vbo(0), numVerts(0), maxVerts(0), vertices(nullptr) {}

void Mesh::create(int initVerts) {
	numVerts = 0;
	maxVerts = initVerts;

	// Allocation
	vertices = (Vertex*)std::calloc(maxVerts, sizeof(Vertex));
	if (vertices == nullptr) {
		DEBUG_ERROR("Failed to allocate memory for vertices");
		return;
	}

	// Generate VAO and VBO
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Bind
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// VAO remembers how buffer data is structured
	glBufferData(GL_ARRAY_BUFFER, maxVerts * sizeof(Vertex), vertices, GL_DYNAMIC_DRAW);

	// Vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
	glEnableVertexAttribArray(0);

	// Vertex color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Vertex texture atlas coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::destroy() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	std::free(vertices);
}

void Mesh::addVertex(Vertex v) {
	if (numVerts == maxVerts) {
		maxVerts *= 2;
		vertices = (Vertex*)std::realloc(vertices, maxVerts * sizeof(Vertex));
		if (vertices == nullptr) {
			DEBUG_ERROR("Failed to re-allocate memory for vertices");
			return;
		}

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, maxVerts * sizeof(Vertex), vertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		DEBUG_TRACE("Expanding VAO to %d vertices", maxVerts);
	}

	vertices[numVerts] = v;
	numVerts++;
}

void Mesh::clear() {
	numVerts = 0;
}

void Mesh::draw() {
	// Bind
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Draw VAO
	glBufferSubData(GL_ARRAY_BUFFER, 0, numVerts * sizeof(Vertex), vertices);
	glDrawArrays(GL_TRIANGLES, 0, numVerts);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Unbind resources
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

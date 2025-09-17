#include <rs/mesh.hpp>
#include <glad/glad.h>
#include <cstdlib>

using namespace rs;

Mesh::Mesh() : vao(0), vbo(0), numVerts(0), maxVerts(0), vertices(nullptr) {}

void Mesh::create(int initVerts) {
	numVerts = 0;
	maxVerts = initVerts;

	// Allocation
	vertices = (Vertex*)std::calloc(maxVerts, sizeof(Vertex));
	CRITICAL_ASSERT(vertices != nullptr, "Failed to allocate memory for vertices");

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Vertex texture atlas coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Vertex normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::destroy() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	std::free(vertices);
}

void Mesh::addVertex(const Vertex& v) {
	if (numVerts == maxVerts) {
		maxVerts *= 2;
		vertices = (Vertex*)std::realloc(vertices, maxVerts * sizeof(Vertex));
		CRITICAL_ASSERT(vertices != nullptr, "Failed to re-allocate memory for vertices");

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

void Mesh::addGeometry(const ge::GeometryData& gd) {
	for (auto i : gd.indices) {
		addVertex({gd.positions[i], gd.colors[i], gd.uvs[i], gd.normals[i]});
	}
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

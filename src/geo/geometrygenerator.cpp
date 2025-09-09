#include <geo/geometrygenerator.hpp>

using namespace geo;

GeometryData GeometryGenerator::GenerateCube(lin::Vec3 color) {
	GeometryData gd;

	const lin::Vec3 positions[] = {
		// Front face
		{-0.5f, -0.5f,  0.5f},
		{ 0.5f, -0.5f,  0.5f},
		{ 0.5f,  0.5f,  0.5f},
		{-0.5f,  0.5f,  0.5f},
		// Back face
		{ 0.5f, -0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f},
		{-0.5f,  0.5f, -0.5f},
		{ 0.5f,  0.5f, -0.5f},
		// Left face
		{-0.5f, -0.5f, -0.5f},
		{-0.5f, -0.5f,  0.5f},
		{-0.5f,  0.5f,  0.5f},
		{-0.5f,  0.5f, -0.5f},
		// Right face
		{ 0.5f, -0.5f,  0.5f},
		{ 0.5f, -0.5f, -0.5f},
		{ 0.5f,  0.5f, -0.5f},
		{ 0.5f,  0.5f,  0.5f},
		// Top face
		{-0.5f,  0.5f,  0.5f},
		{ 0.5f,  0.5f,  0.5f},
		{ 0.5f,  0.5f, -0.5f},
		{-0.5f,  0.5f, -0.5f},
		// Bottom face
		{-0.5f, -0.5f, -0.5f},
		{ 0.5f, -0.5f, -0.5f},
		{ 0.5f, -0.5f,  0.5f},
		{-0.5f, -0.5f,  0.5f},
	};

	const lin::Vec3 normals[] = {
		// Front
		{ 0,  0,  1},
		{ 0,  0,  1},
		{ 0,  0,  1},
		{ 0,  0,  1},
		// Back
		{ 0,  0, -1},
		{ 0,  0, -1},
		{ 0,  0, -1},
		{ 0,  0, -1},
		// Left
		{-1,  0,  0},
		{-1,  0,  0},
		{-1,  0,  0},
		{-1,  0,  0},
		// Right
		{ 1,  0,  0},
		{ 1,  0,  0},
		{ 1,  0,  0},
		{ 1,  0,  0},
		// Top
		{ 0,  1,  0},
		{ 0,  1,  0},
		{ 0,  1,  0},
		{ 0,  1,  0},
		// Bottom
		{ 0, -1,  0},
		{ 0, -1,  0},
		{ 0, -1,  0},
		{ 0, -1,  0},
	};

	const lin::Vec2 uvs[] = {
		{0, 0}, {1, 0}, {1, 1}, {0, 1},
		{0, 0}, {1, 0}, {1, 1}, {0, 1},
		{0, 0}, {1, 0}, {1, 1}, {0, 1},
		{0, 0}, {1, 0}, {1, 1}, {0, 1},
		{0, 0}, {1, 0}, {1, 1}, {0, 1},
		{0, 0}, {1, 0}, {1, 1}, {0, 1},
	};

	for (int i = 0; i < 24; ++i) {
		gd.positions.push_back(positions[i]);
		gd.normals.push_back(normals[i]);
		gd.uvs.push_back(uvs[i]);
		gd.colors.push_back(color);
	}

	// 6 faces × 2 triangles = 12 triangles = 36 indices
	const uint indices[] = {
		0,  1,  2,  2,  3,  0,      // Front
		4,  5,  6,  6,  7,  4,      // Back
		8,  9, 10, 10, 11,  8,      // Left
		12, 13, 14, 14, 15, 12,     // Right
		16, 17, 18, 18, 19, 16,     // Top
		20, 21, 22, 22, 23, 20,     // Bottom
	};

	gd.indices.insert(gd.indices.end(), std::begin(indices), std::end(indices));
	return gd;
}

GeometryData GeometryGenerator::GenerateCube() {
	return GenerateCube({1, 1, 1});
}


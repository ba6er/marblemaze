#include <geo/geometrygenerator.hpp>
#include <cmath>

using namespace geo;

GeometryData GeometryGenerator::GenerateCube(lin::Vec3 color, CubeFaceMask faces) {
	GeometryData gd;
	uint vertexOffset = 0;

	constexpr lin::Vec3 positions[] = {
		{-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},
		{ 0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f},
		{ 0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f},
		{-0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f},
	};
	constexpr lin::Vec3 normals[] = {
		{0, 0, 1}, {0, 0, -1}, {-1, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, -1, 0},
	};
	constexpr lin::Vec2 uvs[] = {
		{0, 0}, {1, 0}, {1, 1}, {0, 1},
	};
	constexpr uint faceIndices[] = {0, 1, 2, 2, 3, 0};

	for (int face = 0; face < 6; face++) {
		if ((faces & (1 << face)) == 0) {
			continue;
		}
		for (int i = 0; i < 4; i++) {
			gd.positions.push_back(positions[face * 4 + i]);
			gd.normals.push_back(normals[face]);
			gd.uvs.push_back(uvs[i]);
			gd.colors.push_back(color);
		}
		for (int fi = 0; fi < 6; fi++) {
			gd.indices.push_back(vertexOffset + faceIndices[fi]);
		}
		vertexOffset += 4;
	}

	return gd;
}

GeometryData GeometryGenerator::GenerateCube(CubeFaceMask faces) {
	return GenerateCube({1, 1, 1}, faces);
}

GeometryData GeometryGenerator::GenerateIcosphere(lin::Vec3 color, int subdivisions) {
	// Sphere with radius of 0.5, with a golden ratio square inside for an icosahedron
	constexpr float cx = 0.425325404175785f;
	constexpr float cy = 0.262865556059567f;

	constexpr lin::Vec3 corners[] = {
		{ cx,  cy,  0 }, { 0,   cx, -cy}, { 0,   cx,  cy},
		{ cy,  0,  -cx}, { cy,  0,   cx}, { cx, -cy,  0 },
		{-cy,  0,  -cx}, {-cx,  cy,  0 }, {-cy,  0,   cx},
		{ 0,  -cx, -cy}, { 0,  -cx,  cy}, {-cx, -cy,  0 },
	};
	constexpr int indices[] = {
		0,  1,  2,    0,  3,  1,    0,  2,  4,    3,  0,  5,
		0,  4,  5,    1,  3,  6,    1,  7,  2,    7,  1,  6,
		4,  2,  8,    7,  8,  2,    9,  3,  5,    6,  3,  9,
		5,  4,  10,   4,  8,  10,   9,  5,  10,   7,  6,  11,
		7,  11, 8,    11, 6,  9,    8,  11, 10,   10, 11, 9,
	};

	GeometryData gd;
	for (int i = 0; i < 3 * 4; i++) {
			lin::Vec3 nc = corners[i] * 2;
			lin::Vec2 uv = { // Spherical projection
				0.5f + std::atan2(nc.z, nc.x) / (2 * lin::Pi),
				0.5f - std::asin(nc.y) / lin::Pi
			};
			gd.positions.push_back(corners[i]);
			gd.normals.push_back(nc);
			gd.uvs.push_back(uv);
			gd.colors.push_back(color);
	}
	for (int i = 0; i < 3 * 4 * 5; i++) {
		gd.indices.push_back(indices[i]);
	}

	if (subdivisions == 0) {
		return gd;
	}
	return gd;
}

GeometryData GeometryGenerator::GenerateIcosphere(int subdivisions) {
	return GenerateIcosphere({1, 1, 1}, subdivisions);
}


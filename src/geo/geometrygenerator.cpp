#include <geo/geometrygenerator.hpp>

using namespace geo;

GeometryData GeometryGenerator::GenerateCube(lin::Vec3 color, CubeFaceMask faces) {
	GeometryData gd;
	uint vertexOffset = 0;

	const lin::Vec3 positions[] = {
		{-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},
		{ 0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f},
		{ 0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f},
		{-0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f},
	};
	const lin::Vec3 normals[] = {
		{0, 0, 1}, {0, 0, -1}, {-1, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, -1, 0},
	};
	const lin::Vec2 uvs[] = {
		{0, 0}, {1, 0}, {1, 1}, {0, 1},
	};
	const uint faceIndices[] = {0, 1, 2, 2, 3, 0};

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


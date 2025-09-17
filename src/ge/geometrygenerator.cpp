#include <ge/geometrygenerator.hpp>
#include <cmath>

using namespace ge;

GeometryData GeometryGenerator::GenerateCube(la::Vec3 color, CubeFaceMask faces) {
	GeometryData gd;
	uint vertexOffset = 0;

	constexpr la::Vec3 positions[] = {
		{-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},
		{ 0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f},
		{ 0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f},
		{-0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f},
	};
	constexpr la::Vec3 normals[] = {
		{0, 0, 1}, {0, 0, -1}, {-1, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, -1, 0},
	};
	constexpr la::Vec2 uvs[] = {
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

GeometryData GeometryGenerator::GenerateIcosphere(la::Vec3 color, int subdivisions) {
	// Golden ratio square used for an icosahedron
	constexpr float g = 1.61803398875f;

	constexpr la::Vec3 corners[] = {
		{ g,  1,  0}, { 0,  g, -1}, { 0,  g,  1},
		{ 1,  0, -g}, { 1,  0,  g}, { g, -1,  0},
		{-1,  0, -g}, {-g,  1,  0}, {-1,  0,  g},
		{ 0, -g, -1}, { 0, -g,  1}, {-g, -1,  0},
	};
	constexpr int indices[] = {
		0,  1,  2,    0,  3,  1,    0,  2,  4,    3,  0,  5,
		0,  4,  5,    1,  3,  6,    1,  7,  2,    7,  1,  6,
		4,  2,  8,    7,  8,  2,    9,  3,  5,    6,  3,  9,
		5,  4,  10,   4,  8,  10,   9,  5,  10,   7,  6,  11,
		7,  11, 8,    11, 6,  9,    8,  11, 10,   10, 11, 9,
	};

	std::vector<Triangle> triangles;
	for (int i = 0; i < 20; i++) {
		Triangle t = {corners[indices[i * 3]], corners[indices[i * 3 + 1]], corners[indices[i * 3 + 2]]};
		AppendSubdivedTriangle(triangles, t, subdivisions);
	}

	GeometryData gd;
	for (int i = 0; i < triangles.size(); i++) {
		for (int j = 0; j < 3; j++) {
			la::Vec3 nc = triangles[i][j].normalize();
			la::Vec2 uv = { // Spherical projection
				0.5f + std::atan2(nc.z, nc.x) / (2 * la::Pi),
				0.5f - std::asin(nc.y) / la::Pi
			};
			gd.indices.push_back(i * 3 + j);
			gd.positions.push_back(nc / 2);
			gd.normals.push_back(nc);
			gd.uvs.push_back(uv);
			gd.colors.push_back(color);
		}
	}
	return gd;
}

GeometryData GeometryGenerator::GenerateIcosphere(int subdivisions) {
	return GenerateIcosphere({1, 1, 1}, subdivisions);
}

GeometryGenerator::SubTriangle GeometryGenerator::SubdivideTriangle(const Triangle& t) {
	la::Vec3 m[] = {(t[0] + t[1]) / 2, (t[1] + t[2]) / 2, (t[2] + t[0]) / 2};
	Triangle t1 = {m[2], t[0], m[0]};
	Triangle t2 = {m[0], t[1], m[1]};
	Triangle t3 = {m[1], t[2], m[2]};
	Triangle t4 = {m[0], m[1], m[2]};
	return {t1, t2, t3, t4};
}

void GeometryGenerator::AppendSubdivedTriangle(std::vector<Triangle>& tv, const Triangle& t, int subdivisions) {
	if (subdivisions == 0) {
		tv.push_back(t);
		return;
	}
	SubTriangle tArr = SubdivideTriangle(t);
	for (auto& st : tArr) {
		AppendSubdivedTriangle(tv, st, subdivisions - 1);
	}
}


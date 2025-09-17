#include <ge/geometrydata.hpp>

using namespace ge;

GeometryData GeometryData::operator+(const GeometryData& gd) const {
	std::vector<la::Vec3> newPositions = positions;
	newPositions.insert(newPositions.end(), gd.positions.begin(), gd.positions.end());

	std::vector<la::Vec3> newColors = colors;
	newColors.insert(newColors.end(), gd.colors.begin(), gd.colors.end());

	std::vector<la::Vec2> newUVs = uvs;
	newUVs.insert(newUVs.end(), gd.uvs.begin(), gd.uvs.end());

	std::vector<la::Vec3> newNormals = normals;
	newNormals.insert(newNormals.end(), gd.normals.begin(), gd.normals.end());

	std::vector<uint> newIndices = indices;
	newIndices.insert(newIndices.end(), gd.indices.begin(), gd.indices.end());
	uint indexOffset = positions.size() - gd.positions.size();
	for (uint i = newIndices.size() - gd.indices.size(); i < newIndices.size(); i++) {
		newIndices[i] += indexOffset;
	}

	return {newPositions, newColors, newUVs, newNormals, newIndices};
}

void GeometryData::operator+=(const GeometryData& gd) {
	positions.insert(positions.end(), gd.positions.begin(), gd.positions.end());
	colors.insert(colors.end(), gd.colors.begin(), gd.colors.end());
	uvs.insert(uvs.end(), gd.uvs.begin(), gd.uvs.end());
	normals.insert(normals.end(), gd.normals.begin(), gd.normals.end());

	uint indexOffset = positions.size() - gd.positions.size();
	indices.insert(indices.end(), gd.indices.begin(), gd.indices.end());
	for (uint i = indices.size() - gd.indices.size(); i < indices.size(); i++) {
		indices[i] += indexOffset;
	}
}

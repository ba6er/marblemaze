#include <geo/geometrydata.hpp>

using namespace geo;

GeometryData GeometryData::operator+(const GeometryData& gd) const {
	std::vector<lin::Vec3> newPositions = positions;
	newPositions.insert(newPositions.end(), gd.positions.begin(), gd.positions.end());

	std::vector<lin::Vec3> newColors = colors;
	newColors.insert(newColors.end(), gd.colors.begin(), gd.colors.end());

	std::vector<lin::Vec2> newUVs = uvs;
	newUVs.insert(newUVs.end(), gd.uvs.begin(), gd.uvs.end());

	std::vector<lin::Vec3> newNormals = normals;
	newNormals.insert(newNormals.end(), gd.normals.begin(), gd.normals.end());

	std::vector<uint> newIndicies = indicies;
	newIndicies.insert(newIndicies.end(), gd.indicies.begin(), gd.indicies.end());
	for (uint i = indicies.size(); i < newIndicies.size(); i++) {
		newIndicies[i] += indicies.size();
	}

	return {newPositions, newColors, newUVs, newNormals, newIndicies};
}

void GeometryData::operator+=(const GeometryData& gd) {
	positions.insert(positions.end(), gd.positions.begin(), gd.positions.end());
	colors.insert(colors.end(), gd.colors.begin(), gd.colors.end());
	uvs.insert(uvs.end(), gd.uvs.begin(), gd.uvs.end());
	normals.insert(normals.end(), gd.normals.begin(), gd.normals.end());
	uint indexOffset = indicies.size();
	indicies.insert(indicies.end(), gd.indicies.begin(), gd.indicies.end());
	for (uint i = indexOffset; i < indicies.size(); i++) {
		indicies[i] += indexOffset;
	}
}


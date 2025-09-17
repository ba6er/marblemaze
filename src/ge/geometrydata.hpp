#pragma once
#include <la/linalg.hpp>
#include <util.hpp>
#include <vector>

namespace ge {

	class GeometryData {

	public:
		std::vector<la::Vec3> positions;
		std::vector<la::Vec3> colors;
		std::vector<la::Vec2> uvs;
		std::vector<la::Vec3> normals;

		std::vector<uint> indices;

		GeometryData operator+(const GeometryData& gd) const;
		void operator+=(const GeometryData& gd);
	};
}

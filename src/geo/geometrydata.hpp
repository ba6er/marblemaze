#pragma once
#include <lin/linalg.hpp>
#include <util.hpp>
#include <vector>

namespace geo {

	class GeometryData {

	public:
		std::vector<lin::Vec3> positions;
		std::vector<lin::Vec3> colors;
		std::vector<lin::Vec2> uvs;
		std::vector<lin::Vec3> normals;

		std::vector<uint> indices;

		GeometryData operator+(const GeometryData& gd) const;
		void operator+=(const GeometryData& gd);
	};
}

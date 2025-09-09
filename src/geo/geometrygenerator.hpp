#pragma once
#include <geo/geometrydata.hpp>

namespace geo {

	class GeometryGenerator {

	public:
		static GeometryData GenerateCube(lin::Vec3 color);
		static GeometryData GenerateCube();
	};
}

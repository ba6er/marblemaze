#pragma once
#include <geo/geometrydata.hpp>

namespace geo {

	enum CubeFace : uint {
		Front  = 1 << 0,
		Back   = 1 << 1,
		Left   = 1 << 2,
		Right  = 1 << 3,
		Top    = 1 << 4,
		Bottom = 1 << 5,
		All    = Front | Back | Left | Right | Top | Bottom,
	};
	using CubeFaceMask = uint;

	class GeometryGenerator {

	public:
		static GeometryData GenerateCube(lin::Vec3 color, CubeFaceMask faces = CubeFace::All);
		static GeometryData GenerateCube(CubeFaceMask faces = CubeFace::All);
	};
}

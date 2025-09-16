#pragma once
#include <geo/geometrydata.hpp>
#include <array>

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
		static GeometryData GenerateIcosphere(lin::Vec3 color, int subdivisions = 0);
		static GeometryData GenerateIcosphere(int subdivisions = 0);

	private:
		using Triangle = std::array<lin::Vec3, 3>;
		using SubTriangle = std::array<Triangle, 4>;

		static SubTriangle SubdivideTriangle(const Triangle& t);
		static void AppendSubdivedTriangle(std::vector<Triangle>& tv, const Triangle& t, int subdivisions);
	};
}

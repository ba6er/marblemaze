#pragma once
#include <geo/geometrydata.hpp>

namespace geo {

	class GeometryTransform {

	public:
		static void ApplyMatrix(GeometryData& gd, const lin::Mat4& matrix);

		static void Scale(GeometryData& gd, lin::Vec3 scale);
		static void Rotate(GeometryData& gd, float angle, lin::Vec3 axis);
		static void Translate(GeometryData& gd, lin::Vec3 translation);

		static void ApplyColor(GeometryData& gd, lin::Vec3 color);
	};
}

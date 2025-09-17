#pragma once
#include <ge/geometrydata.hpp>

namespace ge {

	class GeometryTransform {

	public:
		static void ApplyMatrix(GeometryData& gd, const la::Mat4& matrix);

		static void Scale(GeometryData& gd, la::Vec3 scale);
		static void Rotate(GeometryData& gd, float angle, la::Vec3 axis);
		static void Translate(GeometryData& gd, la::Vec3 translation);

		static void ApplyColor(GeometryData& gd, la::Vec3 color);
	};
}

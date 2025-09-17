#include <ge/geometrytransform.hpp>

using namespace ge;

void GeometryTransform::ApplyMatrix(GeometryData& gd, const la::Mat4& matrix) {
	for (auto& p : gd.positions) {
		p = matrix * p;
	}
	la::Mat4 inverseTransposeMatrix = matrix.inverse().transpose();
	for (auto& n : gd.normals) {
		n = (inverseTransposeMatrix * n).normalize();
	}
}

void GeometryTransform::Scale(GeometryData& gd, la::Vec3 scale) {
	ApplyMatrix(gd, la::Mat4::Scale(scale));
}

void GeometryTransform::Rotate(GeometryData& gd, float angle, la::Vec3 axis) {
	ApplyMatrix(gd, la::Mat4::Rotate(angle, axis));
}

void GeometryTransform::Translate(GeometryData& gd, la::Vec3 translation) {
	ApplyMatrix(gd, la::Mat4::Translate(translation));
}

void GeometryTransform::ApplyColor(GeometryData& gd, la::Vec3 color) {
	for (auto& c : gd.colors) {
		c = color;
	}
}


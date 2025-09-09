#include <geo/geometrytransform.hpp>

using namespace geo;

void GeometryTransform::ApplyMatrix(GeometryData& gd, const lin::Mat4& matrix) {
	for (auto& p : gd.positions) {
		p = matrix * p;
	}
	lin::Mat4 inverseTransposeMatrix = matrix.inverse().transpose();
	for (auto& n : gd.normals) {
		n = (inverseTransposeMatrix * n).normalize();
	}
}

void GeometryTransform::Scale(GeometryData& gd, lin::Vec3 scale) {
	ApplyMatrix(gd, lin::Mat4::Scale(scale));
}

void GeometryTransform::Rotate(GeometryData& gd, float angle, lin::Vec3 axis) {
	ApplyMatrix(gd, lin::Mat4::Rotate(angle, axis));
}

void GeometryTransform::Translate(GeometryData& gd, lin::Vec3 translation) {
	ApplyMatrix(gd, lin::Mat4::Translate(translation));
}

void GeometryTransform::ApplyColor(GeometryData& gd, lin::Vec3 color) {
	for (auto& c : gd.colors) {
		c = color;
	}
}


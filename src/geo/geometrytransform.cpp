#include <geo/geometrytransform.hpp>

using namespace geo;

void GeometryTransform::ApplyMatrix(GeometryData& gd, const lin::Mat4& matrix) {
	for (int i = 0; i < gd.positions.size(); i++) {
		gd.positions[i] = matrix * gd.positions[i];
	}
	lin::Mat4 inverseTransposeMatrix = matrix.inverse().transpose();
	for (int i = 0; i < gd.normals.size(); i++) {
		gd.normals[i] = inverseTransposeMatrix * gd.normals[i];
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


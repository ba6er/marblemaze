#include <gm/marble.hpp>
#include <ge/geometrygenerator.hpp>
#include <ge/geometrytransform.hpp>

using namespace gm;

Marble::Marble() : Marble({0, 0, 0}) {}

Marble::Marble(la::Vec3 position) : Marble(position, {0, 0, 0}) {}

Marble::Marble(la::Vec3 position, la::Vec3 velocity) : Marble(position, velocity, 0.25f) {}

Marble::Marble(la::Vec3 position, la::Vec3 velocity, float radius)
	: transform(la::Mat4::Identity())
	, position(position), velocity(velocity), direction({0, 0, 0})
	, radius(radius), speed(0), acceleration(0) {}

ge::GeometryData Marble::toGeometry() const {
	ge::GeometryData gd = ge::GeometryGenerator::GenerateIcosphere(2);
	ge::GeometryTransform::Scale(gd, {radius * 2, radius * 2, radius * 2});
	return gd;
}

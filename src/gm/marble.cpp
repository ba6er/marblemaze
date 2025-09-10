#include <gm/marble.hpp>
#include <geo/geometrygenerator.hpp>
#include <geo/geometrytransform.hpp>

using namespace gm;

Marble::Marble() : Marble({0, 0, 0}) {}

Marble::Marble(lin::Vec3 position) : Marble(position, {0, 0, 0}) {}

Marble::Marble(lin::Vec3 position, lin::Vec3 velocity) : Marble(position, velocity, 0) {}

Marble::Marble(lin::Vec3 position, lin::Vec3 velocity, float radius)
	: position(position), velocity(velocity), radius(radius) {}

geo::GeometryData Marble::toGeometry() const
{
	auto gd = geo::GeometryGenerator::GenerateCube();
	geo::GeometryTransform::Scale(gd, {radius / 2, radius / 2, radius / 2});
	geo::GeometryTransform::Translate(gd, position);
	return gd;
}

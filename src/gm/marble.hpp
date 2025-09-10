#pragma once
#include <geo/geometrydata.hpp>
#include <lin/linalg.hpp>

namespace gm {

	class Marble {

	public:
		Marble();
		Marble(lin::Vec3 position);
		Marble(lin::Vec3 position, lin::Vec3 velocity);
		Marble(lin::Vec3 position, lin::Vec3 velocity, float radius);

		geo::GeometryData toGeometry() const;

		lin::Vec3 position;
		lin::Vec3 velocity;
		float radius;
	};
}

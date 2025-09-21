#pragma once
#include <ge/geometrydata.hpp>
#include <la/linalg.hpp>

namespace gm {

	class Marble {

	public:
		Marble();
		Marble(la::Vec3 position);
		Marble(la::Vec3 position, la::Vec3 velocity);
		Marble(la::Vec3 position, la::Vec3 velocity, float radius);

		ge::GeometryData toGeometry() const;

		la::Mat4 transform;

		la::Vec3 position;
		la::Vec3 velocity;
		la::Vec3 direction;
		float radius, speed, acceleration;
	};
}

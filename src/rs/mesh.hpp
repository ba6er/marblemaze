#pragma once
#include <ge/geometrydata.hpp>
#include <la/linalg.hpp>
#include <util.hpp>

namespace rs {

	class Vertex {

	public:
		la::Vec3 position;
		la::Vec3 color;
		la::Vec2 atlas;
		la::Vec3 normal;
	};

	class Mesh {

	public:
		Mesh();

		void create(int initVerts);
		void destroy();

		void addVertex(const Vertex& v);
		void addGeometry(const ge::GeometryData& gd);

		void clear();
		void draw();

	private:
		uint vao, vbo;
		int  numVerts, maxVerts;
		Vertex* vertices;
	};
}


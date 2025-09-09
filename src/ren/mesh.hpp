#pragma once
#include <geo/geometrydata.hpp>
#include <lin/linalg.hpp>
#include <util.hpp>

namespace ren {

	class Vertex {

	public:
		lin::Vec3 position;
		lin::Vec4 color;
		lin::Vec2 atlas;
		lin::Vec3 normal;
	};

	class Mesh {

	public:
		Mesh();

		void create(int initVerts = 6);
		void destroy();

		void addVertex(const Vertex& v);
		void addGeometry(const geo::GeometryData& gd);

		void clear();
		void draw();

	private:
		uint vao, vbo;
		int  numVerts, maxVerts;
		Vertex* vertices;
	};
}


#pragma once
#include <lin/linalg.hpp>
#include <util.hpp>

namespace ren {

	class Vertex {

	public:
		lin::Vec3 position;
		lin::Vec4 color;
		lin::Vec2 atlas;
	};

	class Mesh {

	public:
		void create(int initVerts = 6);
		void destroy();

		void addVertex(Vertex v);

		void clear();
		void draw();

	private:
		uint vao, vbo;
		int  numVerts, maxVerts;
		Vertex* vertices;
	};
}


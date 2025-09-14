#pragma once
#include <geo/geometrydata.hpp>
#include <util.hpp>
#include <string_view>
#include <vector>

namespace gm {

	enum BlockType : uchar {
		Empty = 0,
		Wall,
		Start,
		Finish,
	};

	class Maze {

	public:
		Maze();
		Maze(int width, int height, int depth);

		void loadFromFile(std::string_view fileName);

		BlockType getBlock(int x, int y, int z) const;
		void setBlock(int x, int y, int z, BlockType block);

		geo::GeometryData toGeometry() const;

		int getWidth() const;
		int getHeight() const;
		int getDepth() const;

		lin::Mat4 transform;

	private:
		inline int index(int x, int y, int z) const {
			return x + width * (z + depth * y);
		}

		int width, height, depth;
		std::vector<BlockType> blocks;
	};
}

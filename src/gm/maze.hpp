#pragma once
#include <ge/geometrydata.hpp>
#include <util.hpp>
#include <string>
#include <vector>

namespace gm {

	using BlockVector3D = std::vector<std::vector<std::string>>;

	enum BlockType : uchar {
		Empty  = 0,
		Wall   = 1 << 0,
		Start  = 1 << 1,
		Finish = 1 << 2,
	};

	class Maze {

	public:

		Maze();
		Maze(int width, int height, int depth);

		void create(const BlockVector3D& initBlocks);

		BlockType getBlock(int x, int y, int z) const;
		void setBlock(int x, int y, int z, BlockType block);

		ge::GeometryData toGeometry() const;

		int getWidth() const;
		int getHeight() const;
		int getDepth() const;

		la::Mat4 transform;

	private:
		inline int index(int x, int y, int z) const {
			return x + width * (z + depth * y);
		}

		int width, height, depth;
		std::vector<BlockType> blocks;
	};
}

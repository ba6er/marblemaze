#include <gm/maze.hpp>
#include <ge/geometrygenerator.hpp>
#include <ge/geometrytransform.hpp>

using namespace gm;

Maze::Maze() : Maze(1, 1, 1) {}

Maze::Maze(int width, int height, int depth)
	: transform(la::Mat4::Identity())
	, width(width), height(height), depth(depth)
	, blocks(width * height * depth, BlockType::Empty) {}

void Maze::create(const BlockVector3D& initBlocks) {
	height = initBlocks.size();
	depth = initBlocks[0].size();
	width = initBlocks[0][0].size();

	blocks = std::vector(width * height * depth, BlockType::Empty);

	for (int y = 0; y < height; y++) {
		for (int z = 0; z < depth; z++) {
			for (int x = 0; x < width; x++) {
				BlockType b = Empty;
				if (initBlocks[y][z][x] == 'W') {
					b = Wall;
				}
				else if (initBlocks[y][z][x] == 's') {
					b = Start;
				}
				else if (initBlocks[y][z][x] == 'f') {
					b = Finish;
				}
				setBlock(x, height - 1 - y, z, b);
			}
		}
	}
}

BlockType Maze::getBlock(int x, int y, int z) const {
	if (x < 0 || x >= width || y < 0 || y >= height || z < 0 || z >= depth) {
		DEBUG_WARNING("Block %d %d %d is out of bounds", x, y, z);
		return Empty;
	}
	return blocks.at(index(x, y, z));
}

void Maze::setBlock(int x, int y, int z, BlockType block) {
	if (x < 0 || x >= width || y < 0 || y >= height || z < 0 || z >= depth) {
		DEBUG_WARNING("Block %d %d %d is out of bounds", x, y, z);
		return;
	}
	blocks[index(x, y, z)] = block;
}

ge::GeometryData Maze::toGeometry() const {
	ge::GeometryData fullData;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			for (int z = 0; z < depth; z++) {
				if (getBlock(x, y, z) == Wall) {
					ge::CubeFaceMask faces = ge::CubeFace::All;
					if ((x != 0) && getBlock(x - 1, y, z) == Wall) {
						faces &= ~(ge::CubeFace::Left);
					}
					if ((x != width - 1) && getBlock(x + 1, y, z) == Wall) {
						faces &= ~(ge::CubeFace::Right);
					}
					if (y != 0 && getBlock(x, y - 1, z) == Wall) {
						faces &= ~(ge::CubeFace::Bottom);
					}
					if ((y != height - 1) && getBlock(x, y + 1, z) == Wall) {
						faces &= ~(ge::CubeFace::Top);
					}
					if ((z != 0) && getBlock(x, y, z - 1) == Wall) {
						faces &= ~(ge::CubeFace::Back);
					}
					if ((z != depth - 1) && getBlock(x, y, z + 1) == Wall) {
						faces &= ~(ge::CubeFace::Front);
					}
					ge::GeometryData cube = ge::GeometryGenerator::GenerateCube(faces);
					la::Vec3 position = {(float)x, (float)y, (float)z};
					ge::GeometryTransform::Translate(cube, position);
					fullData += cube;
				}
			}
		}
	}
	return fullData;
}

int Maze::getWidth() const {
	return width;
}

int Maze::getHeight() const {
	return height;
}

int Maze::getDepth() const {
	return depth;
}

#include <gm/maze.hpp>
#include <geo/geometrygenerator.hpp>
#include <geo/geometrytransform.hpp>
#include <fstream>

using namespace gm;

Maze::Maze() : Maze(1, 1, 1) {}

Maze::Maze(int width, int height, int depth)
	: transform(lin::Mat4::Identity())
	, width(width), height(height), depth(depth)
	, blocks(width * height * depth, BlockType::Empty) {}

void Maze::loadFromFile(std::string_view fileName) {
	std::ifstream in(fileName.data());
	if (in.is_open() == false) {
		DEBUG_ERROR("No maze file by the name of %s", fileName.data());
		return;
	}

	in >> width >> depth >> height;
	blocks = std::vector(width * height * depth, BlockType::Empty);

	for (int y = 0; y < height; y++) {
		for (int z = 0; z < depth; z++) {
			std::string line = "";
			do {
				std::getline(in, line);
			} while (line.length() == 0);
			if (line.length() != width) {
				DEBUG_WARNING("Line %s doesn't have the with of %d", line.c_str(), width);
			}
			for (int x = 0; x < width; x++) {
				BlockType b = Empty;
				if (line[x] == '#') {
					b = Wall;
				}
				setBlock(x, y, z, b);
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

geo::GeometryData Maze::toGeometry() const {
	geo::GeometryData fullData;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			for (int z = 0; z < depth; z++) {
				if (getBlock(x, y, z) == Wall) {
					auto cube = geo::GeometryGenerator::GenerateCube();
					lin::Vec3 position = {(float)x, (float)y, (float)z};
					geo::GeometryTransform::Translate(cube, position);
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

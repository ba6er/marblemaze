#include <gm/scene.hpp>
#include <cmath>
#include <algorithm>

using namespace gm;

void Scene::updatePhysics(float deltaTime) {
	marble.position += marble.velocity * deltaTime;

	int minX = std::floor(marble.position.x - marble.radius);
	int maxX = std::ceil(marble.position.x + marble.radius);
	int minY = std::floor(marble.position.y - marble.radius);
	int maxY = std::ceil(marble.position.y + marble.radius);
	int minZ = std::floor(marble.position.z - marble.radius);
	int maxZ = std::ceil(marble.position.z + marble.radius);

	if (minX >= maze.getWidth() || minY >= maze.getHeight() || minZ >= maze.getDepth()) {
		return;
	}
	if (maxX < 0 || maxY < 0 || maxZ < 0) {
		return;
	}
	while (minX < 0 && maxX >= 0) { minX++; }
	while (minY < 0 && maxY >= 0) { minY++; }
	while (minZ < 0 && maxZ >= 0) { minZ++; }
	while (maxX >= maze.getWidth() && minX < maze.getWidth()) { maxX--; }
	while (maxY >= maze.getHeight() && minY < maze.getHeight()) { maxY--; }
	while (maxZ >= maze.getDepth() && minZ < maze.getDepth()) { maxZ--; }

	for (int y = minY; y <= maxY; ++y) {
		for (int z = minZ; z <= maxZ; ++z) {
			for (int x = minX; x <= maxX; ++x) {
				if (maze.getBlock(x, y, z) != BlockType::Wall) {
					continue;
				}
				marbleBlockCollision(x, y, z);
			}
		}
	}
}

void Scene::marbleBlockCollision(float x, float y, float z) {

}

#include <gm/scene.hpp>
#include <cmath>
#include <algorithm>

using namespace gm;

void Scene::updatePhysics(float deltaTime) {
	marble.position += marble.velocity * deltaTime;

	int minX = (int)(std::floor(marble.position.x - marble.radius));
	int maxX = (int)(std::ceil(marble.position.x + marble.radius));
	int minY = (int)(std::floor(marble.position.y - marble.radius));
	int maxY = (int)(std::ceil(marble.position.y + marble.radius));
	int minZ = (int)(std::floor(marble.position.z - marble.radius));
	int maxZ = (int)(std::ceil(marble.position.z + marble.radius));

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

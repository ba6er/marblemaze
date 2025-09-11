#include <gm/scene.hpp>
#include <cmath>
#include <algorithm>

using namespace gm;

void Scene::updatePhysics(float deltaTime) {

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

	// DEBUG_TRACE("%d %d %d %d %d %d", minX, maxX, minY, maxY, minZ, maxZ);

	for (int y = minY; y <= maxY; y++) {
		for (int z = minZ; z <= maxZ; z++) {
			for (int x = minX; x <= maxX; x++) {
				if (maze.getBlock(x, y, z) != BlockType::Wall) {
					continue;
				}
				marbleBlockCollision(x, y, z, deltaTime);
			}
		}
	}
	marble.position += marble.velocity * deltaTime;
}

void Scene::marbleBlockCollision(float x, float y, float z, float deltaTime) {
	lin::Vec3 newPosX = marble.position + (lin::Vec3){marble.velocity.x * deltaTime, 0, 0};
	float disX = DistanceSphereAABB({x, y, z}, newPosX);
	if (disX < marble.radius) {
		marble.velocity.x = 0;
	}

	lin::Vec3 newPosY = marble.position + (lin::Vec3){0, marble.velocity.y * deltaTime, 0};
	float disY = DistanceSphereAABB({x, y, z}, newPosY);
	if (disY < marble.radius) {
		marble.velocity.y = 0;
	}

	lin::Vec3 newPosZ = marble.position + (lin::Vec3){0, 0, marble.velocity.z * deltaTime};
	float disZ = DistanceSphereAABB({x, y, z}, newPosZ);
	if (disZ < marble.radius) {
		marble.velocity.z = 0;
	}
}

float Scene::DistanceSphereAABB(lin::Vec3 box, lin::Vec3 sphere) {
	float minPos[3] = {box.x - 0.5f, box.y - 0.5f, box.z - 0.5f};
	float maxPos[3] = {box.x + 0.5f, box.y + 0.5f, box.z + 0.5f};
	float sphPos[3] = {sphere.x, sphere.y, sphere.z};

	// Sphere-AABB collision
	float distanceSquared = 0;
	for (int i = 0; i < 3; i++) {
		if (sphPos[i] < minPos[i]) {
			distanceSquared += (minPos[i] - sphPos[i]) * (minPos[i] - sphPos[i]);
			continue;
		}
		if (sphPos[i] > maxPos[i]) {
			distanceSquared += (sphPos[i] - maxPos[i]) * (sphPos[i] - maxPos[i]);
			continue;
		}
	}
	return std::sqrt(distanceSquared);
}


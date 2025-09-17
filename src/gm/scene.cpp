#include <gm/scene.hpp>
#include <cmath>

using namespace gm;

Scene::Scene()
		: camera(), renderables()
		, marble(), maze()
		, cameraDistance(10), cameraYaw(0), cameraPitch(0)
		, marbleIsTouchingWalls({false, false, false}), marbleWasTouchingWalls({false, false, false}) {
	light = {{0}};
}

void Scene::updateCamera() {
	updateCamera(0, 0, 0);
}


void Scene::updateCamera(float deltaYaw, float deltaPitch, float deltaDistance) {
	cameraYaw += deltaYaw;
	cameraPitch = std::min(std::max(cameraPitch + deltaPitch, -la::Pi / 2 + 0.1f), la::Pi / 2 - 0.1f);

	cameraDistance += deltaDistance;
	if (cameraDistance <= 0) {
		cameraDistance = 0.1f;
	}

	la::Vec3 offset = camera.getTarget();
	la::Vec3 newPosition = {
		offset.x + std::cos(cameraPitch) * std::cos(cameraYaw) * cameraDistance,
		offset.y + std::sin(cameraPitch) * cameraDistance,
		offset.z + std::cos(cameraPitch) * std::sin(cameraYaw) * cameraDistance,
	};
	camera.setPosition(newPosition);
}

void Scene::updateMazeRotation(float deltaYaw, float deltaRoll) {
	la::Vec3 mazeCenter = {
		(int)(maze.getWidth() / 2) * 1.0f,
		(int)(maze.getHeight() / 2) * 1.0f,
		(int)(maze.getDepth() / 2) * 1.0f,
	};
	maze.transform = maze.transform * la::Mat4::Rotate(deltaYaw, {-std::sin(cameraYaw), 0, std::cos(cameraYaw)});
	maze.transform = maze.transform * la::Mat4::Rotate(deltaRoll, {std::cos(cameraYaw), 0, std::sin(cameraYaw)});
	marble.transform = marble.transform * la::Mat4::Rotate(-deltaYaw, {-std::sin(cameraYaw), 0, std::cos(cameraYaw)});
	marble.transform = marble.transform * la::Mat4::Rotate(-deltaRoll, {std::cos(cameraYaw), 0, std::sin(cameraYaw)});

	la::Mat4 nt = la::Mat4::Translate(mazeCenter) * maze.transform * la::Mat4::Translate(-mazeCenter);
	renderables[0].transform = nt;
}

void Scene::updatePhysics(float deltaTime) {
	marble.velocity = marble.transform * (la::Vec3){0, -marble.speed, 0};

	int minX = std::floor(marble.position.x - marble.radius);
	int maxX = std::ceil(marble.position.x + marble.radius);
	int minY = std::floor(marble.position.y - marble.radius);
	int maxY = std::ceil(marble.position.y + marble.radius);
	int minZ = std::floor(marble.position.z - marble.radius);
	int maxZ = std::ceil(marble.position.z + marble.radius);

	if (minX >= maze.getWidth() || minY >= maze.getHeight() || minZ >= maze.getDepth()) {
		marble.position += marble.velocity * deltaTime;
		return;
	}
	if (maxX < 0 || maxY < 0 || maxZ < 0) {
		marble.position += marble.velocity * deltaTime;
		return;
	}
	while (minX < 0 && maxX >= 0) { minX++; }
	while (minY < 0 && maxY >= 0) { minY++; }
	while (minZ < 0 && maxZ >= 0) { minZ++; }
	while (maxX >= maze.getWidth() && minX < maze.getWidth()) { maxX--; }
	while (maxY >= maze.getHeight() && minY < maze.getHeight()) { maxY--; }
	while (maxZ >= maze.getDepth() && minZ < maze.getDepth()) { maxZ--; }

	// DEBUG_TRACE("%d %d %d %d %d %d", minX, maxX, minY, maxY, minZ, maxZ);

	marbleWasTouchingWalls = marbleIsTouchingWalls;
	marbleIsTouchingWalls = {false, false, false};
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

bool Scene::shouldPlaySound() {
	bool tx = marbleWasTouchingWalls[0] != marbleIsTouchingWalls[0] && marbleIsTouchingWalls[0];
	bool ty = marbleWasTouchingWalls[1] != marbleIsTouchingWalls[1] && marbleIsTouchingWalls[1];
	bool tz = marbleWasTouchingWalls[2] != marbleIsTouchingWalls[2] && marbleIsTouchingWalls[2];
	return tx || ty || tz;
}

void Scene::marbleBlockCollision(float x, float y, float z, float deltaTime) {
	la::Vec3 newPosX = marble.position + (la::Vec3){marble.velocity.x * deltaTime, 0, 0};
	float disX = DistanceSphereAABB({x, y, z}, newPosX);
	if (disX < marble.radius) {
		marble.velocity.x = 0;
		marbleIsTouchingWalls[0] = true;
	}

	la::Vec3 newPosY = marble.position + (la::Vec3){0, marble.velocity.y * deltaTime, 0};
	float disY = DistanceSphereAABB({x, y, z}, newPosY);
	if (disY < marble.radius) {
		marble.velocity.y = 0;
		marbleIsTouchingWalls[1] = true;
	}

	la::Vec3 newPosZ = marble.position + (la::Vec3){0, 0, marble.velocity.z * deltaTime};
	float disZ = DistanceSphereAABB({x, y, z}, newPosZ);
	if (disZ < marble.radius) {
		marble.velocity.z = 0;
		marbleIsTouchingWalls[2] = true;
	}
}

float Scene::DistanceSphereAABB(la::Vec3 box, la::Vec3 sphere) {
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


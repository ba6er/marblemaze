#include <gm/scene.hpp>
#include <ge/geometrygenerator.hpp>
#include <ge/geometrytransform.hpp>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace gm;

Scene::Scene()
		: id(""), timer(0), bestTime(10000), camera(), renderables()
		, marble(), maze(), start({0, 0, 0}), finish({0, 0, 0}), initCameraValues({0, 0, 0})
		, cameraDistance(0), cameraYaw(0), cameraPitch(0)
		, marbleIsTouchingWalls({false, false, false}), marbleWasTouchingWalls({false, false, false}) {
	light = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
}

bool Scene::createFromFile(std::string_view fileName, rs::ResourceManager& resource) {
	std::ifstream configIn(fileName.data());
	if (configIn.is_open() == false) {
		CRITICAL_ERROR("No maze file by the name of %s", fileName.data());
		return false;
	}

	std::string configLine;
	std::string marbleMaterialName, mazeMaterialName, finishMaterialName, skyboxMaterialName;
	size_t mazeWidth = 0, mazeDepth = 0, mazeHeight = 0;
	BlockVector3D initBlocks;
	while (std::getline(configIn, configLine)) {
		if (configLine.length() == 0 || configLine.at(0) == '#') {
			continue;
		}

		std::string configToken;
		std::istringstream configStream(configLine);
		configStream >> configToken;
		if (configToken == "INTERNAL_ID") {
			configStream >> id;
		}
		if (configToken == "LIGHT_POSITION") {
			configStream >> light.position.x >> light.position.y >> light.position.z;
		}
		else if (configToken == "LIGHT_AMBIENT") {
			configStream >> light.ambient.x >> light.ambient.y >> light.ambient.z;
			light.ambient /= 100;
		}
		else if (configToken == "LIGHT_DIFFUSE") {
			configStream >> light.diffuse.x >> light.diffuse.y >> light.diffuse.z;
			light.diffuse /= 100;
		}
		else if (configToken == "LIGHT_SPECULAR") {
			configStream >> light.specular.x >> light.specular.y >> light.specular.z;
			light.specular /= 100;
		}
		else if (configToken == "CAMERA_YAW") {
			configStream >> cameraYaw;
			cameraYaw *= la::DegToRad;
		}
		else if (configToken == "CAMERA_PITCH") {
			configStream >> cameraPitch;
			cameraPitch *= la::DegToRad;
		}
		else if (configToken == "CAMERA_DISTANCE") {
			configStream >> cameraDistance;
		}
		else if (configToken == "MAZE_MATERIAL") {
			configStream >> mazeMaterialName;
		}
		else if (configToken == "MARBLE_MATERIAL") {
			configStream >> marbleMaterialName;
		}
		else if (configToken == "FINISH_MATERIAL") {
			configStream >> finishMaterialName;
		}
		else if (configToken == "SKYBOX_MATERIAL") {
			configStream >> skyboxMaterialName;
		}
		else if (configToken == "MARBLE_DIRECTION") {
			configStream >> marble.direction.x >> marble.direction.y >> marble.direction.z;
			marble.direction = marble.direction.normalize();
		}
		else if (configToken == "MARBLE_ACCELERATION") {
			configStream >> marble.acceleration;
		}
		else if (configToken == "MARBLE_MAX_SPEED") {
			configStream >> marble.speed;
		}
		else if (configToken == "MARBLE_RADIUS") {
			configStream >> marble.radius;
		}
		else if (configToken == "MAZE_WIDTH") {
			configStream >> mazeWidth;
		}
		else if (configToken == "MAZE_DEPTH") {
			configStream >> mazeDepth;
		}
		else if (configToken == "MAZE_HEIGHT") {
			configStream >> mazeHeight;
		}
		else if (configToken == "MAZE_BEGIN") {
			if (mazeWidth == 0 || mazeHeight == 0 || mazeDepth == 0) {
				CRITICAL_ERROR("Maze dimension aren't properly defined (or are defined after the maze)");
				return false;
			}
			for (size_t i = 0; i < mazeDepth * mazeHeight;) {
				std::string mazeLine = "";
				std::getline(configIn, mazeLine);
				if (mazeLine.length() == 0 || mazeLine.at(0) == '#') {
					continue;
				}
				if (mazeLine == "MAZE_END") {
					CRITICAL_ERROR("Maze ends before it should");
					return false;
				}
				if (mazeLine.size() != mazeWidth) {
					CRITICAL_ERROR("Maze line %s doesn't have %ld elements", mazeLine.c_str(), mazeWidth);
					return false;
				}
				if (i % mazeDepth == 0) {
					initBlocks.push_back(std::vector<std::string>());
				}
				size_t mazeStartPos = mazeLine.find('s');
				if (mazeStartPos != std::string::npos) {
					start = {
						mazeStartPos * 1.0f,
						(int)(mazeHeight - (int)(i / mazeDepth) - 1) * 1.0f,
						(int)(i % mazeDepth) * 1.0f
					};
					marble.position = start;
				}
				size_t mazeFinishPos = mazeLine.find('f');
				if (mazeFinishPos != std::string::npos) {
					finish = {
						mazeFinishPos * 1.0f,
						(int)(mazeHeight - (int)(i / mazeDepth) - 1) * 1.0f,
						(int)(i % mazeDepth) * 1.0f
					};
				}
				initBlocks[i / mazeDepth].push_back(mazeLine);
				i++;
			}
		}
		else if (configToken == "MAZE_END") {
			maze.create(initBlocks);
		}
	}

	la::Vec3 cameraTarget = {
		(int)(maze.getWidth() / 2) * 1.0f,
		(int)(maze.getHeight() / 2) * 1.0f,
		(int)(maze.getDepth() / 2) * 1.0f,
	};
	camera.setTarget(cameraTarget);
	updateCamera();
	initCameraValues = {cameraDistance, cameraYaw, cameraPitch};

	rs::Mesh& mazeMesh = resource.createMesh(id + "_maze", 36 * mazeWidth * mazeHeight * mazeDepth);
	mazeMesh.addGeometry(maze.toGeometry());

	rs::Mesh& marbleMesh = resource.createMesh(id + "_marble", 960);
	marbleMesh.addGeometry(marble.toGeometry());

	ge::GeometryData finishCube = ge::GeometryGenerator::GenerateCube();
	ge::GeometryTransform::Translate(finishCube, finish);
	rs::Mesh& finishMesh = resource.createMesh(id + "_finish", 36);
	finishMesh.addGeometry(finishCube);

	ge::GeometryData skybox = ge::GeometryGenerator::GenerateCube();
	ge::GeometryTransform::Scale(skybox, {1000, 1000, 1000});
	ge::GeometryTransform::Translate(skybox, cameraTarget);
	rs::Mesh& skyboxMesh = resource.createMesh(id + "_skybox", 36);
	skyboxMesh.addGeometry(skybox);

	renderables.push_back(rn::Renderable());
	renderables[0].create(mazeMesh, resource.getMaterial(mazeMaterialName), "maze");
	renderables.push_back(rn::Renderable());
	renderables[1].create(marbleMesh, resource.getMaterial(marbleMaterialName), "marble");
	renderables.push_back(rn::Renderable());
	renderables[2].create(finishMesh, resource.getMaterial(finishMaterialName), "finish");
	renderables.push_back(rn::Renderable());
	renderables[3].create(skyboxMesh, resource.getMaterial(skyboxMaterialName), "skybox");

	return true;
}

void Scene::createMenuScene(rs::ResourceManager& resource) {
	std::string mazeMaterialName = "copperTextured";
	std::string marbleMaterialName = "emerald";
	std::string finishMaterialName = "emerald";
	std::string skyboxMaterialName = "sky";
	size_t mazeWidth = 7, mazeDepth = 7, mazeHeight = 2;
	BlockVector3D initBlocks = {
		{
			"WWW.WWW",
			"W.....W",
			"WWWWW.W",
			"W.....W",
			"W.WW..W",
			"...W..W",
			"WWWWWWW",
		}, {
			"WWWWWWW",
			"WWWWWWW",
			"WWWWWWW",
			"WWWfWWW",
			"WWWWWWW",
			"WWWWWWW",
			"WWWWWWW",
		}
	};
	start = {3, 1, 3};
	finish = {3, 0, 3};

	light = {
		{1.0f, 9.9f, 1.0f},
		{0.3f, 0.3f, 0.3f},
		{1.0f, 1.0f, 1.0f},
		{0.5f, 0.5f, 0.5f},
	};
	cameraYaw = la::Pi / 2;
	cameraPitch = la::Pi / 4;
	cameraDistance = 10;

	maze.create(initBlocks);
	marble.position = start;

	la::Vec3 cameraTarget = {
		(int)(maze.getWidth() / 2) * 1.0f,
		(int)(maze.getHeight() / 2) * 1.0f,
		(int)(maze.getDepth() / 2) * 1.0f,
	};
	camera.setTarget(cameraTarget);
	initCameraValues = {cameraDistance, cameraYaw, cameraPitch};

	rs::Mesh& mazeMesh = resource.createMesh("maze_menu", 36 * mazeWidth * mazeHeight * mazeDepth);
	mazeMesh.addGeometry(maze.toGeometry());

	rs::Mesh& marbleMesh = resource.createMesh("marble_menu", 960);
	marbleMesh.addGeometry(marble.toGeometry());

	ge::GeometryData finishCube = ge::GeometryGenerator::GenerateCube();
	ge::GeometryTransform::Translate(finishCube, finish);
	rs::Mesh& finishMesh = resource.createMesh("finish_menu", 36);
	finishMesh.addGeometry(finishCube);

	ge::GeometryData skybox = ge::GeometryGenerator::GenerateCube();
	ge::GeometryTransform::Scale(skybox, {1000, 1000, 1000});
	ge::GeometryTransform::Translate(skybox, cameraTarget);
	rs::Mesh& skyboxMesh = resource.createMesh("skybox_menu", 36);
	skyboxMesh.addGeometry(skybox);

	renderables.push_back(rn::Renderable());
	renderables[0].create(mazeMesh, resource.getMaterial(mazeMaterialName), "maze");
	renderables.push_back(rn::Renderable());
	renderables[1].create(marbleMesh, resource.getMaterial(marbleMaterialName), "marble");
	renderables.push_back(rn::Renderable());
	renderables[2].create(finishMesh, resource.getMaterial(finishMaterialName), "finish");
	renderables.push_back(rn::Renderable());
	renderables[3].create(skyboxMesh, resource.getMaterial(skyboxMaterialName), "skybox");

	updateCamera();
	updateMazeRotation(0, 0);
}

void Scene::restart() {
	timer = 0;

	cameraDistance = initCameraValues.x;
	cameraYaw = initCameraValues.y;
	cameraPitch = initCameraValues.z;

	marble.position = start;
	marble.velocity = {0, 0, 0};

	marble.transform = la::Mat4::Identity();
	maze.transform = la::Mat4::Identity();

	updateCamera();
	updateMazeRotation(0, 0);

	marbleIsTouchingWalls = {false, false, false};
	marbleWasTouchingWalls = marbleIsTouchingWalls;
}

void Scene::destroy() {
	timer = 0;

	camera = rn::Camera();
	light = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
	renderables.clear();

	marble = Marble();
	maze = Maze();

	start = {0, 0, 0};
	finish = {0, 0, 0};
	initCameraValues = {0, 0, 0};
	cameraDistance = 0;
	cameraYaw = 0;
	cameraPitch = 0;

	marbleIsTouchingWalls = {false, false, false};
	marbleWasTouchingWalls = {false, false, false};
}

void Scene::setCameraValues(float yaw, float pitch, float distance) {
	cameraYaw = yaw;
	cameraPitch = std::min(std::max(pitch, -la::Pi / 2 + 0.1f), la::Pi / 2 - 0.1f);
	cameraDistance = std::max(distance, 0.1f);

	la::Vec3 offset = camera.getTarget();
	la::Vec3 newPosition = {
		offset.x + std::cos(cameraPitch) * std::cos(cameraYaw) * cameraDistance,
		offset.y + std::sin(cameraPitch) * cameraDistance,
		offset.z + std::cos(cameraPitch) * std::sin(cameraYaw) * cameraDistance,
	};
	camera.setPosition(newPosition);
}

void Scene::updateCamera() {
	la::Vec3 offset = camera.getTarget();
	la::Vec3 newPosition = {
		offset.x + std::cos(cameraPitch) * std::cos(cameraYaw) * cameraDistance,
		offset.y + std::sin(cameraPitch) * cameraDistance,
		offset.z + std::cos(cameraPitch) * std::sin(cameraYaw) * cameraDistance,
	};
	camera.setPosition(newPosition);
}

void Scene::updateCamera(float deltaYaw, float deltaPitch, float deltaDistance) {
	setCameraValues(cameraYaw + deltaYaw, cameraPitch + deltaPitch, cameraDistance + deltaDistance);
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
	marble.velocity += marble.transform * marble.direction * marble.acceleration * deltaTime;
	if (marble.velocity.length() > marble.speed) {
		marble.velocity = marble.velocity.normalize() * marble.speed;
	}

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

void Scene::updateTimer(float deltaTime) {
	timer += deltaTime;
}

std::string_view Scene::getId() const {
	return id;
}

bool Scene::checkWinCondition() const {
	return DistanceSphereAABB(finish, marble.position) <= marble.radius;
}

float Scene::getTime() const {
	return timer;
}

float Scene::getBestTime() const {
	return bestTime;
}

void Scene::setBestTime(float bestTime) {
	this->bestTime = bestTime;
}

float Scene::getCameraYaw() const {
	return cameraYaw;
}

float Scene::getCameraPitch() const {
	return cameraPitch;
}

float Scene::getCameraDistance() const {
	return cameraDistance;
}

void Scene::setProjection(float fov, float ratio) {
	camera.project3d(fov, ratio, 0.0001f, 9999.9f);
}

void Scene::display() {
	// Transform marble and finish by maze's transform
	renderables[1].transform = renderables[0].transform * la::Mat4::Translate(marble.position);
	renderables[2].transform = renderables[0].transform;
	rn::Renderer::render(camera, renderables, light);
}

bool Scene::shouldPlaySound() {
	bool tx = marbleWasTouchingWalls[0] != marbleIsTouchingWalls[0] && marbleIsTouchingWalls[0];
	bool ty = marbleWasTouchingWalls[1] != marbleIsTouchingWalls[1] && marbleIsTouchingWalls[1];
	bool tz = marbleWasTouchingWalls[2] != marbleIsTouchingWalls[2] && marbleIsTouchingWalls[2];
	return tx || ty || tz;
}

void Scene::marbleBlockCollision(float x, float y, float z, float deltaTime) {
	la::Vec3 diff = DifferenceSphereAABB({x, y, z}, marble.position + marble.velocity * deltaTime);

	if (diff.x * diff.x + diff.y * diff.y + diff.z * diff.z >= marble.radius * marble.radius) {
		return;
	}

	if (std::abs(diff.x) > marble.radius / 2) {
		bool pushedCorner = false;
		if (std::abs(diff.y) > std::abs(diff.x)) {
			float offY = std::sqrt(marble.radius * marble.radius - diff.x * diff.x) * (diff.y > 0 ? 1 : -1);
			marble.velocity.y -= offY - diff.y;
			pushedCorner = true;
		}
		if (std::abs(diff.z) > std::abs(diff.x)) {
			float offZ = std::sqrt(marble.radius * marble.radius - diff.x * diff.x) * (diff.z > 0 ? 1 : -1);
			marble.velocity.z -= offZ - diff.z;
			pushedCorner = true;
		}
		if (!pushedCorner) {
			marble.velocity.x = 0;
		}
		marbleIsTouchingWalls[0] = true;
	}
	if (std::abs(diff.y) > marble.radius / 2) {
		bool pushedCorner = false;
		if (std::abs(diff.z) > std::abs(diff.y)) {
			float offZ = std::sqrt(marble.radius * marble.radius - diff.x * diff.x) * (diff.z > 0 ? 1 : -1);
			marble.velocity.z -= offZ - diff.z;
			pushedCorner = true;
		}
		if (std::abs(diff.x) > std::abs(diff.y)) {
			float offX = std::sqrt(marble.radius * marble.radius - diff.z * diff.z) * (diff.x > 0 ? 1 : -1);
			marble.velocity.x -= offX - diff.x;
			pushedCorner = true;
		}
		if (!pushedCorner) {
			marble.velocity.y = 0;
		}
		marbleIsTouchingWalls[1] = true;
	}
	if (std::abs(diff.z) > marble.radius / 2) {
		bool pushedCorner = false;
		if (std::abs(diff.y) > std::abs(diff.z)) {
			float offY = std::sqrt(marble.radius * marble.radius - diff.x * diff.x) * (diff.y > 0 ? 1 : -1);
			marble.velocity.y -= offY - diff.y;
			pushedCorner = true;
		}
		if (std::abs(diff.x) > std::abs(diff.z)) {
			float offX = std::sqrt(marble.radius * marble.radius - diff.z * diff.z) * (diff.x > 0 ? 1 : -1);
			marble.velocity.x -= offX - diff.x;
			pushedCorner = true;
		}
		if (!pushedCorner) {
			marble.velocity.z = 0;
		}
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

la::Vec3 Scene::DifferenceSphereAABB(la::Vec3 box, la::Vec3 sphere) {
	float minPos[3] = {box.x - 0.5f, box.y - 0.5f, box.z - 0.5f};
	float maxPos[3] = {box.x + 0.5f, box.y + 0.5f, box.z + 0.5f};
	float sphPos[3] = {sphere.x, sphere.y, sphere.z};
	float diff[3] = {0, 0, 0};

	// Sphere-AABB collision
	for (int i = 0; i < 3; i++) {
		if (sphPos[i] < minPos[i]) {
			diff[i] = minPos[i] - sphPos[i];
			continue;
		}
		if (sphPos[i] > maxPos[i]) {
			diff[i] = sphPos[i] - maxPos[i];
			continue;
		}
	}
	return {diff[0], diff[1], diff[2]};
}

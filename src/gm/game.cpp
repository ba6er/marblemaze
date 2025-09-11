#include <geo/geometrygenerator.hpp>
#include <geo/geometrytransform.hpp>
#include <gm/game.hpp>
#include <ren/renderer.hpp>
#include <cmath>

using namespace gm;

void Game::onInit(int width, int height, ren::RenderAssetManager& ram) {
	ren::Renderer::resizeFrame(width, height);

	currentScene.light = {
		{1.0f, 9.9f, 1.0f},
		{0.3f, 0.3f, 0.3f},
		{1.0f, 1.0f, 1.0f},
		{0.5f, 0.5f, 0.5f},
	};

	currentScene.maze.loadFromFile(_RES_PATH "testLevel.txt");

	currentScene.marble.position = {5, 1, 4};
	currentScene.marble.velocity = {0, -1, 0};

	lin::Vec3 cameraTarget = {
		std::floorf((int)(currentScene.maze.getWidth() / 2)),
		0,
		std::floorf((int)(currentScene.maze.getDepth() / 2)),
	};
	lin::Vec3 cameraPosition = {
		cameraTarget.x + std::cosf(currentScene.cameraAngle) * 6,
		cameraTarget.y + 6,
		cameraTarget.z + std::sinf(currentScene.cameraAngle) * 6,
	};

	currentScene.camera.setPosition(cameraPosition);
	currentScene.camera.setTarget(cameraTarget);
	currentScene.camera.project3d(72 * lin::DegToRad, (float)width / (float)height, 0.001f, 999.9f);

	auto& maze = ram.createMesh("maze");
	maze.create(6 * 6 * 9 * 9 * 2);
	maze.addGeometry(currentScene.maze.toGeometry());

	currentScene.renderables.push_back(ren::Renderable());
	currentScene.renderables[0].create(maze, ram.getMaterial("copperTextured"));

	auto& marble = ram.createMesh("marble");
	marble.create(6 * 6);
	marble.addGeometry(currentScene.marble.toGeometry());

	currentScene.renderables.push_back(ren::Renderable());
	currentScene.renderables[1].create(marble, ram.getMaterial("emerald"));

	auto skybox = geo::GeometryGenerator::GenerateCube();
	geo::GeometryTransform::Scale(skybox, {1000, 1000, 1000});
	geo::GeometryTransform::Translate(skybox, cameraTarget);

	auto& sky = ram.createMesh("sky");
	sky.create(6 * 6);
	sky.addGeometry(skybox);

	currentScene.renderables.push_back(ren::Renderable());
	currentScene.renderables[2].create(sky, ram.getMaterial("sky"));
}

void Game::onResize(int width, int height) {
	ren::Renderer::resizeFrame(width, height);
	currentScene.camera.project3d(72 * lin::DegToRad, (float)width / (float)height, 0.001f, 999.9f);
}

bool Game::onUpdate(float deltaTime, float currentTime, const in::Input& input) {
	if (input.getKey(in::Quit) == in::JustReleased) {
		return false;
	}

	int pressed[6] = {
		input.getKey(in::MazePitchIncrease) == in::Pressed,
		input.getKey(in::MazePitchDecrease) == in::Pressed,
		input.getKey(in::MazeYawIncrease) == in::Pressed,
		input.getKey(in::MazeYawDecrease) == in::Pressed,
		input.getKey(in::CameraAngleIncrease) == in::Pressed,
		input.getKey(in::CameraAngleDecrease) == in::Pressed,
	};

	if (pressed[3] || pressed[2] || pressed[1] || pressed[0]) {
		currentScene.mazePitch += (pressed[1] - pressed[0]) * deltaTime;
		currentScene.mazeYaw += (pressed[3] - pressed[2]) * deltaTime;
		lin::Vec3 mazeCenter = {
			currentScene.maze.getWidth() / 2.0f,
			currentScene.maze.getHeight() / 2.0f,
			currentScene.maze.getDepth() / 2.0f,
		};
		lin::Mat4 nt = lin::Mat4::Translate(mazeCenter);
		nt = nt * lin::Mat4::Rotate(currentScene.mazePitch, {0, 0, 1});
		nt = nt * lin::Mat4::Rotate(currentScene.mazeYaw, {0, 1, 0});
		nt = nt * lin::Mat4::Translate(-mazeCenter);
		currentScene.renderables[0].transform = nt;
	}

	if (pressed[5] || pressed[4]) {
		currentScene.cameraAngle += (pressed[5] - pressed[4]) * deltaTime;
		lin::Vec3 target = currentScene.camera.getTarget();
		lin::Vec3 cameraPosition = {
			target.x + std::cosf(currentScene.cameraAngle) * 6,
			target.y + 6,
			target.z + std::sinf(currentScene.cameraAngle) * 6,
		};
		currentScene.camera.setPosition(cameraPosition);
	}

	lin::Vec3 marbleDirection = {
		std::sinf(currentScene.mazePitch) * std::cosf(currentScene.mazeYaw),
		-std::cosf(currentScene.mazePitch),
		-std::sinf(currentScene.mazePitch) * std::sinf(currentScene.mazeYaw),
	};
	currentScene.marble.velocity = marbleDirection * 2;

	currentScene.updatePhysics(deltaTime);

	return true;
}

void Game::onRender(float deltaTime, float currentTime, ren::RenderAssetManager& ram) {
	lin::Mat4 marTr = currentScene.renderables[0].transform;
	marTr = marTr * lin::Mat4::Translate(currentScene.marble.position);
	currentScene.renderables[1].transform = marTr;

	ren::Renderer::clear(0.5f, 0.5f, 0.5f);
	ren::Renderer::render(currentScene.camera, currentScene.renderables, currentScene.light);
}

#include <geo/geometrygenerator.hpp>
#include <geo/geometrytransform.hpp>
#include <gm/game.hpp>
#include <ren/renderer.hpp>
#include <cmath>

using namespace gm;

void Game::onInit(int width, int height, ren::RenderAssetManager& ram) {
	ren::Renderer::resizeFrame(width, height);

	currentScene.rotator = lin::Mat4::Identity();

	currentScene.light = {
		{1.0f, 9.9f, 1.0f},
		{0.3f, 0.3f, 0.3f},
		{1.0f, 1.0f, 1.0f},
		{0.5f, 0.5f, 0.5f},
	};

	currentScene.maze.loadFromFile(_RES_PATH "testLevel.txt");

	currentScene.marble.position = {5, 1, 4};
	currentScene.marble.speed = 2;

	currentScene.cameraYaw = 0;
	currentScene.cameraPitch = lin::Pi / 4;
	currentScene.cameraDistance = 10;

	lin::Vec3 cameraTarget = {
		std::floorf((int)(currentScene.maze.getWidth() / 2)),
		0,
		std::floorf((int)(currentScene.maze.getDepth() / 2)),
	};
	currentScene.camera.setTarget(cameraTarget);
	currentScene.camera.project3d(72 * lin::DegToRad, (float)width / (float)height, 0.001f, 999.9f);
	currentScene.updateCamera();

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

	int pressed[] = {
		input.getKey(in::MazeYawDecrease) == in::Pressed,
		input.getKey(in::MazeYawIncrease) == in::Pressed,
		input.getKey(in::MazeRollDecrease) == in::Pressed,
		input.getKey(in::MazeRollIncrease) == in::Pressed,
	};

	if (pressed[3] || pressed[2] || pressed[1] || pressed[0]) {
		currentScene.updateMazeRotation(
			(pressed[1] - pressed[0]) * deltaTime,
			(pressed[3] - pressed[2]) * deltaTime);
	}

	if (input.getMouseL() == in::Pressed) {
		currentScene.cameraYaw += input.getDeltaMouseX() * deltaTime * 0.2f;
		float np = currentScene.cameraPitch + input.getDeltaMouseY() * deltaTime * 0.2f;
		currentScene.cameraPitch = std::min(std::max(np, -lin::Pi / 2 + 0.1f), lin::Pi / 2 - 0.1f);
		currentScene.updateCamera();
	}

	if (input.getScrollY() != 0) {
		currentScene.cameraDistance -= input.getScrollY() * deltaTime * 10;
		currentScene.updateCamera();
	}

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

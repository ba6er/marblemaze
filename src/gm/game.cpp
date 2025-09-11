#include <geo/geometrytransform.hpp>
#include <gm/game.hpp>
#include <ren/renderer.hpp>
#include <cmath>

using namespace gm;

void Game::onInit(int width, int height, ren::RenderAssetManager& ram) {
	ren::Renderer::resizeFrame(width, height);

	currentScene.camera.setPosition({8, 6, 4});
	currentScene.camera.setTarget({4, 0, 4});
	currentScene.camera.project3d(72 * lin::DegToRad, (float)width / (float)height, 0.001f, 999.9f);

	currentScene.light = {
		{1.0f, 9.9f, 1.0f},
		{0.3f, 0.3f, 0.3f},
		{1.0f, 1.0f, 1.0f},
		{0.5f, 0.5f, 0.5f},
	};

	currentScene.maze.loadFromFile(_RES_PATH "testLevel.txt");

	currentScene.marble.position = {5, 1, 4};
	currentScene.marble.velocity = {0, -1, 0};

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
		input.getKey(in::CameraAngleIncrease) == in::Pressed,
		input.getKey(in::CameraAngleDecrease) == in::Pressed,
		input.getKey(in::MazeYawIncrease) == in::Pressed,
		input.getKey(in::MazeYawDecrease) == in::Pressed,
	};

	currentScene.marble.velocity.x = (pressed[1] - pressed[0]);
	currentScene.marble.velocity.y = (pressed[3] - pressed[2]);
	currentScene.marble.velocity.z = (pressed[5] - pressed[4]);

	currentScene.updatePhysics(deltaTime);

	return true;
}

void Game::onRender(float deltaTime, float currentTime, ren::RenderAssetManager& ram) {
	currentScene.renderables[1].transform = lin::Mat4::Translate(currentScene.marble.position);

	ren::Renderer::clear(0.5f, 0.5f, 0.5f);
	ren::Renderer::render(currentScene.camera, currentScene.renderables, currentScene.light);
}

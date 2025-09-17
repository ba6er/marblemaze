#include <ge/geometrygenerator.hpp>
#include <ge/geometrytransform.hpp>
#include <gm/game.hpp>
#include <rn/renderer.hpp>

using namespace gm;

Game::Game() : gui(), scene() {}

void Game::onInit(int width, int height, rs::ResourceManager& resource) {
	rn::Renderer::resizeFrame(width, height);

	auto& guiMesh = resource.createMesh("gui");
	guiMesh.create();
	gui.create(resource.getShader("text"), resource.getFont("noto48"), guiMesh);
	gui.setFrame(0, width, 0, height);
	gui.addLabel("test").create(48, "Marble Maze", {width / 2.0f, 32, 0});

	scene.light = {
		{1.0f, 9.9f, 1.0f},
		{0.3f, 0.3f, 0.3f},
		{1.0f, 1.0f, 1.0f},
		{0.5f, 0.5f, 0.5f},
	};

	scene.maze.loadFromFile(_RES_PATH "testLevel.txt");

	scene.marble.position = {5, 1, 4};
	scene.marble.speed = 2;

	scene.cameraYaw = 0;
	scene.cameraPitch = la::Pi / 4;
	scene.cameraDistance = scene.maze.getWidth() / 2 + 5;

	la::Vec3 cameraTarget = {
		(int)(scene.maze.getWidth() / 2) * 1.0f,
		(int)(scene.maze.getHeight() / 2) * 1.0f,
		(int)(scene.maze.getDepth() / 2) * 1.0f,
	};
	scene.camera.setTarget(cameraTarget);
	scene.camera.project3d(72 * la::DegToRad, (float)width / (float)height, 0.001f, 999.9f);
	scene.updateCamera();

	auto& maze = resource.createMesh("maze");
	maze.create(6 * 6 * 9 * 9 * 2);
	maze.addGeometry(scene.maze.toGeometry());

	scene.renderables.push_back(rn::Renderable());
	scene.renderables[0].create(maze, resource.getMaterial("copperTextured"));

	auto& marble = resource.createMesh("marble");
	marble.create(20 * 16 * 3);
	marble.addGeometry(scene.marble.toGeometry());

	scene.renderables.push_back(rn::Renderable());
	scene.renderables[1].create(marble, resource.getMaterial("emerald"));

	auto skybox = ge::GeometryGenerator::GenerateCube();
	ge::GeometryTransform::Scale(skybox, {1000, 1000, 1000});
	ge::GeometryTransform::Translate(skybox, cameraTarget);

	auto& sky = resource.createMesh("sky");
	sky.create(6 * 6);
	sky.addGeometry(skybox);

	scene.renderables.push_back(rn::Renderable());
	scene.renderables[2].create(sky, resource.getMaterial("sky"));
}

void Game::onResize(int width, int height) {
	rn::Renderer::resizeFrame(width, height);
	scene.camera.project3d(72 * la::DegToRad, (float)width / (float)height, 0.001f, 999.9f);

	float ox = (720 - width * 480 / height) / 2;
	gui.setFrame(ox, 720 - ox, 0, 480);
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
		scene.updateMazeRotation(
			(pressed[1] - pressed[0]) * deltaTime,
			(pressed[3] - pressed[2]) * deltaTime);
	}

	if (input.getMouseL() == in::Pressed || input.getScrollY() != 0) {
		scene.updateCamera(
			input.getDeltaMouseX() * deltaTime * 0.2f,
			input.getDeltaMouseY() * deltaTime * 0.2f,
			-input.getScrollY() * deltaTime * 10);
	}

	scene.updatePhysics(deltaTime);

	return true;
}

void Game::onRender(float deltaTime, float currentTime, rs::ResourceManager& resource) {
	la::Mat4 marTr = scene.renderables[0].transform;
	marTr = marTr * la::Mat4::Translate(scene.marble.position);
	scene.renderables[1].transform = marTr;

	rn::Renderer::clear(0.5f, 0.5f, 0.5f);
	rn::Renderer::render(scene.camera, scene.renderables, scene.light);

	gui.display();
}

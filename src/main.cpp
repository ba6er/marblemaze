#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <in/input.hpp>
#include <gm/game.hpp>
#include <ren/renderassetmanager.hpp>
#include <util.hpp>

static void callbackError(int code, const char* text) {
	DEBUG_WARNING("GLFW error: %d, %s", code, text);
}

static const char* title = "Marble Maze";
static const int width  = 720;
static const int height = 480;
static const int fps    = 60;

static GLFWwindow* window;

static gm::Game game;
static in::Input input;
static ren::RenderAssetManager ram;

int main() {
	DEBUG_TRACE("Initializing system");

	// Initialize GLFW
	glfwInit();
	glfwSetErrorCallback(callbackError);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the GLFW window
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == nullptr) {
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	// Loading OpenGL functions via glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}
	DEBUG_TRACE("OpenGL version: %s", glGetString(GL_VERSION));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	ram.initFromConfig(_RES_PATH "assets.txt");
	game.onInit(width, height, ram);

	float tickTime     = 1.0f / fps;
	float currentTime  = 0.0f;
	float previousTime = 0.0f;
	float deltaTime    = 0.0f;
	float lagTime      = 0.0f;
	bool  isRunning    = true;

	int frameWidth = width;
	int frameHeight = height;

	// Main loop
	while(isRunning && !glfwWindowShouldClose(window)) {
		// Update time
		currentTime  = (float)glfwGetTime();
		deltaTime    = currentTime - previousTime;
		previousTime = currentTime;
		lagTime     += deltaTime;

		// Input
		for (int i = 0; i < in::KEYBOARD_COUNT; i++) {
			int keyState = glfwGetKey(window, in::Input::KeyboardValues[i]);
			if (input.getKey(i) == keyState) {
				continue;
			}
			if (keyState == GLFW_PRESS) {
				input.setKey(i, input.getKey(i) == in::JustPressed ? in::Pressed : in::JustPressed);
			} else {
				input.setKey(i, input.getKey(i) == in::JustReleased ? in::Released : in::JustReleased);
			}
		}
		int mouseLeftState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		if (input.getMouseL() != mouseLeftState) {
			if (mouseLeftState == GLFW_PRESS) {
				input.setMouseL(input.getMouseL() == in::JustPressed ? in::Pressed : in::JustPressed);
			} else {
				input.setMouseL(input.getMouseL() == in::JustReleased ? in::Released : in::JustReleased);
			}
		}
		int mouseRightState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		if (input.getMouseR() != mouseRightState) {
			if (mouseRightState == GLFW_PRESS) {
				input.setMouseR(input.getMouseR() == in::JustPressed ? in::Pressed : in::JustPressed);
			} else {
				input.setMouseR(input.getMouseR() == in::JustReleased ? in::Released : in::JustReleased);
			}
		}
		double mouseX, mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		input.setMousePos(mouseX, mouseY);

		// Frame size
		int newFrameWidth, newFrameHeight;
		glfwGetFramebufferSize(window, &newFrameWidth, &newFrameHeight);
		if (frameWidth != newFrameWidth || frameHeight != newFrameHeight) {
			frameWidth = newFrameWidth;
			frameHeight = newFrameHeight;
			game.onResize(frameWidth, frameHeight);
		}

		// Update and render the game
		while (lagTime > tickTime) {
			isRunning = game.onUpdate(tickTime, currentTime, input);
			lagTime -= tickTime;
		}
		game.onRender(deltaTime, currentTime, ram);

		// Update system
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	FreeSystemResources();
	return 0;
}

// Declared in util.hpp
void FreeSystemResources() {
	DEBUG_TRACE("Freeing system resources");

	// Free render assets
	ram.destroy();

	// Free GLFW objects
	glfwDestroyWindow(window);
	glfwTerminate();
}

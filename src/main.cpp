#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <miniaudio/miniaudio.h>
#include <in/input.hpp>
#include <gm/game.hpp>
#include <rs/resourcemanager.hpp>
#include <util.hpp>
#include <array>
#include <fstream>

static void callbackError(int code, const char* text) {
	DEBUG_WARNING("GLFW error: %d, %s", code, text);
}

static const char* title = "Marble Maze";
static const int fps     = 60;
static const int defaultWidth  = 640;
static const int defaultHeight = 480;

static ma_engine audioEngine;
static GLFWwindow* window;

static gm::Game game;
static in::Input input;
static rs::ResourceManager resource;

static void callbackScroll(GLFWwindow*, double x, double y) {
	input.setScroll(x, y);
}

int main() {
	DEBUG_TRACE("This is a debug build");
	CRITICAL_TRACE("Initializing system");

	// Load configuration file
	int width, height;
	std::array<int, 5> initOptionWhileValues;

	try {
		std::ifstream configIn(_RES_PATH "options.txt");
		if (!configIn.is_open()) {
			throw 1;
		}
		if (!(configIn >> width)) {
			throw 1;
		}
		if (width < 0 || width > 8196) {
			throw 1;
		}
		if (!(configIn >> height)) {
			throw 1;
		}
		if (height < 0 || height > 8196) {
			throw 1;
		}
		for (int i = 0; i < 5; i++) {
			if (!(configIn >> initOptionWhileValues[i])) {
				throw 1;
			}
			if (initOptionWhileValues[i] < 1 || initOptionWhileValues[i] > 10) {
				throw 1;
			}
		}
	} catch(...) {
		DEBUG_WARNING("Failed to read from options file, loading from default configuration");
		width = defaultWidth;
		height = defaultHeight;
		initOptionWhileValues = {5, 5, 5, 5, 5};
	}

	// Initialize miniaudio
	if (ma_engine_init(nullptr, &audioEngine) != MA_SUCCESS) {
		return 1;
	}

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
		ma_engine_uninit(&audioEngine);
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetScrollCallback(window, callbackScroll);

	// Loading OpenGL functions via glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		ma_engine_uninit(&audioEngine);
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}
	DEBUG_TRACE("OpenGL version: %s", glGetString(GL_VERSION));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	resource.initFromConfig(_RES_PATH "assets.txt", &audioEngine);
	game.onInit(width, height, 640, 480, resource, initOptionWhileValues);

	float tickTime     = 1.0f / fps;
	float currentTime  = 0.0f;
	float previousTime = 0.0f;
	float deltaTime    = 0.0f;
	float lagTime      = 0.0f;
	bool  isRunning    = true;

	// Main loop
	while(isRunning && !glfwWindowShouldClose(window)) {
		// Update time
		currentTime  = (float)glfwGetTime();
		deltaTime    = currentTime - previousTime;
		previousTime = currentTime;
		lagTime     += deltaTime;

		// Input
		for (int i = 0; i < in::KeyboardCount; i++) {
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
		if (width != newFrameWidth || height != newFrameHeight) {
			width = newFrameWidth;
			height = newFrameHeight;
			game.onResize(width, height);
		}

		// Update and render the game
		while (lagTime > tickTime) {
			isRunning = game.onUpdate(tickTime, currentTime, resource, input);
			lagTime -= tickTime;
		}
		game.onRender(deltaTime, currentTime, resource);

		// Update system
		glfwSwapBuffers(window);
		input.setScroll(0, 0);
		glfwPollEvents();
	}

	// Write new configuration file
	try {
		std::ofstream configOut(_RES_PATH "options.txt");
		configOut << width << std::endl << height << std::endl;
		for (int i = 0; i < 5; i++) {
			configOut << game.optionWholeValues[i] << std::endl;
		}
	} catch(...) {
		DEBUG_WARNING("Failed to write to options file");
	}

	FreeSystemResources();
	return 0;
}

// Declared in util.hpp
void FreeSystemResources() {
	CRITICAL_TRACE("Freeing system resources");

	// Free render assets
	resource.destroy();

	// Free GLFW objects
	glfwDestroyWindow(window);
	glfwTerminate();

	// Free miniaudio objects
	ma_engine_uninit(&audioEngine);
}

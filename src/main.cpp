#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <miniaudio/miniaudio.h>
#include <in/input.hpp>
#include <gm/game.hpp>
#include <rs/resourcemanager.hpp>
#include <util.hpp>
#include <array>
#include <fstream>

constexpr static char title[] = "Marble Maze";
constexpr static int  fps     = 60;
constexpr static int  defaultWidth  = 640;
constexpr static int  defaultHeight = 480;

static ma_engine audioEngine;
static GLFWwindow* window;
static la::Vec2 frameSize;

static rs::ResourceManager& resource() {
	static rs::ResourceManager instance;
	return instance;
}

static in::Input& input() {
	static in::Input instance;
	return instance;
}

static gm::Game& game() {
	static gm::Game instance;
	return instance;
}

static void callbackError(int code, const char* text) {
	DEBUG_WARNING("GLFW error: %d, %s", code, text);
}

static void callbackFramebufferSize(GLFWwindow*, int width, int height) {
	frameSize = {(float)width, (float)height};
	game().onResize(width, height);
}

static void callbackCursorPos(GLFWwindow*, double x, double y) {
	input().setMousePos(x, y);
}

static void callbackScroll(GLFWwindow*, double x, double y) {
	input().setScroll(x, y);
}

template <typename T>
inline T readAndValidate(std::istream& in, T min, T max, std::string_view name) {
	T value;
	if (!(in >> value)) {
		throw std::runtime_error(std::string("Failed to read ") + name.data());
	}
	if (value < min || value > max) {
		throw std::runtime_error(std::string("Invalid value for ") + name.data());
	}
	return value;
}

int main() {
	DEBUG_TRACE("This is a debug build");
	CRITICAL_TRACE("Initializing system");

	// Load configuration file
	gm::GameOptions gameOptions;

	try {
		std::ifstream configIn(_RES_PATH "options.txt");
		if (!configIn.is_open()) {
			throw std::runtime_error("Could not open options file");
		}
		gm::GameOptions opts;

		opts.rememberValues = readAndValidate<int>(configIn, 0, 1, "rememberValues");
		opts.isFullscreen   = readAndValidate<int>(configIn, 0, 1, "isFullscreen");
		frameSize.x = readAndValidate<float>(configIn, 0, 8196, "frameSize.x");
		frameSize.y = readAndValidate<float>(configIn, 0, 8196, "frameSize.y");

		for (size_t i = 0; i < gm::GameOptions::NumWholeValues; i++) {
			opts.wholeValues[i] = readAndValidate<int>(
				configIn,
				gm::GameOptions::WholeMin,
				gm::GameOptions::WholeMax,
				"wholeValues[" + std::to_string(i) + "]"
			);
		}

		gameOptions = opts;
	} catch(const std::runtime_error& e) {
		DEBUG_WARNING("Failed to read from options file, error: %s", e.what());
		frameSize = {defaultWidth, defaultHeight};
	}

	if (gameOptions.rememberValues == 0) {
		frameSize = {defaultWidth, defaultHeight};
		gameOptions = gm::GameOptions();
		gameOptions.rememberValues = 0;
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
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWmonitor* monitor = gameOptions.isFullscreen ? glfwGetPrimaryMonitor() : nullptr;
	if (gameOptions.isFullscreen) {
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		frameSize.x = mode->width;
		frameSize.y = mode->height;
	}
	window = glfwCreateWindow(frameSize.x, frameSize.y, title, monitor, nullptr);
	if (window == nullptr) {
		ma_engine_uninit(&audioEngine);
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, callbackFramebufferSize);
	glfwSetCursorPosCallback(window, callbackCursorPos);
	glfwSetScrollCallback(window, callbackScroll);

	// Loading OpenGL functions via glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		ma_engine_uninit(&audioEngine);
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}
	DEBUG_TRACE("OpenGL version: %s", glGetString(GL_VERSION));

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	resource().initFromConfig(_RES_PATH "assets.txt", &audioEngine);
	game().onInit(frameSize, {640, 480}, resource(), gameOptions);

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

		// Activate input callbacks
		input().setMousePos(input().getMouseX(), input().getMouseY());
		input().setScroll(0, 0);
		glfwPollEvents();

		// Fullscreen check
		if (game().options.isFullscreen != gameOptions.isFullscreen) {
			gameOptions.isFullscreen = game().options.isFullscreen;
			if (gameOptions.isFullscreen) {
				monitor = glfwGetPrimaryMonitor();
				const GLFWvidmode* mode = glfwGetVideoMode(monitor);
				frameSize = {(float)mode->width, (float)mode->height};
				glfwSetWindowSize(window, mode->width, mode->height);
			} else {
				monitor = nullptr;
			}
			glfwSetWindowMonitor(window, monitor, 0, 0, frameSize.x, frameSize.y, GLFW_DONT_CARE);
			game().onResize(frameSize.x, frameSize.y);
		}

		// Update and render the game
		while (lagTime > tickTime) {
			if (input().getMouseL() == in::JustPressed) {
				input().setMouseL(in::Pressed);
			}
			if (input().getMouseL() == in::JustReleased) {
				input().setMouseL(in::Released);
			}
			int mouseLState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
			if (input().getMouseL() != mouseLState) {
				in::KeyState desired = input().getMouseL() == in::JustPressed ? in::Pressed : in::JustPressed;
				if (mouseLState == GLFW_RELEASE) {
					desired = input().getMouseL() == in::JustReleased ? in::Released : in::JustReleased;
				}
				input().setMouseL(desired);
			}

			if (input().getMouseR() == in::JustPressed) {
				input().setMouseR(in::Pressed);
			}
			if (input().getMouseR() == in::JustReleased) {
				input().setMouseR(in::Released);
			}
			int mouseRState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
			if (input().getMouseR() != mouseRState) {
				in::KeyState desired = input().getMouseR() == in::JustPressed ? in::Pressed : in::JustPressed;
				if (mouseRState == GLFW_RELEASE) {
					desired = input().getMouseR() == in::JustReleased ? in::Released : in::JustReleased;
				}
				input().setMouseR(desired);
			}

			for (int i = 0; i < in::KeyboardCount; i++) {
				int keyState = glfwGetKey(window, in::Input::KeyboardValues[i]);
				if (input().getKey(i) == keyState) {
					continue;
				}
				in::KeyState desired = input().getKey(i) == in::JustPressed ? in::Pressed : in::JustPressed;
				if (keyState == GLFW_RELEASE) {
					desired = input().getKey(i) == in::JustReleased ? in::Released : in::JustReleased;
				}
				input().setKey(i, desired);
			}

			isRunning = game().onUpdate(tickTime, currentTime, resource(), input());
			lagTime -= tickTime;
		}
		game().onRender(deltaTime, currentTime, resource());
		glfwSwapBuffers(window);
	}

	// Write scores
	try {
		std::ofstream scoresOut(_RES_PATH "scores.txt");
		scoresOut << game().getScenes().size() << std::endl;
		for (const gm::Scene& s : game().getScenes()) {
			scoresOut << s.getId() << std::endl;
			scoresOut << s.getBestTime() << std::endl;
		}
	} catch(...) {
		DEBUG_WARNING("Failed to write scene scores");
	}

	gameOptions.rememberValues = game().options.rememberValues;
	if (gameOptions.rememberValues == 0) {
		gameOptions = gm::GameOptions();
		gameOptions.rememberValues = 0;
		frameSize = {defaultWidth, defaultHeight};
	} else {
		gameOptions.wholeValues = game().options.wholeValues;
	}

	// Write new configuration file
	try {
		std::ofstream configOut(_RES_PATH "options.txt");
		configOut.exceptions(std::ofstream::failbit | std::ofstream::badbit);
		configOut << gameOptions.rememberValues << std::endl;
		configOut << gameOptions.isFullscreen << std::endl;
		configOut << frameSize.x << std::endl;
		configOut << frameSize.y << std::endl;
		for (size_t i = 0; i < gm::GameOptions::NumWholeValues; i++) {
			configOut << gameOptions.wholeValues[i] << std::endl;
		}
	} catch(const std::ios_base::failure& e) {
		DEBUG_WARNING("Failed to write to options file, error: %s", e.what());
	}

	FreeSystemResources();
	return 0;
}

// Declared in util.hpp
void FreeSystemResources() {
	CRITICAL_TRACE("Freeing system resources");

	// Free render assets
	resource().destroy();

	// Free GLFW objects
	glfwDestroyWindow(window);
	glfwTerminate();

	// Free miniaudio objects
	ma_engine_uninit(&audioEngine);
}

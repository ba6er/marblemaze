#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <in/input.hpp>
#include <gm/game.hpp>
#include <ren/renderassetmanager.hpp>
#include <util.hpp>

constexpr cstr title  = "Marble Maze";
constexpr int  width  = 720;
constexpr int  height = 480;
constexpr int  fps    = 60;

static void callbackError(int code, cstr text) {
	DEBUG_WARNING("GLFW error: %d, %s", code, text);
}

int main() {
	// Initialize GLFW
	glfwInit();
	glfwSetErrorCallback(callbackError);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
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

	gm::Game game;
	in::Input input;
	ren::RenderAssetManager ram;

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

	ram.destroy();

	// Free GLFW objects
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

#pragma once
#include <GLFW/glfw3.h>

namespace in {

	enum KeyState {
		Released     = GLFW_RELEASE,
		Pressed      = GLFW_PRESS,
		JustReleased = 2,
		JustPressed  = 3,
	};

	enum KeyName {
		Quit,
		MazeRollDecrease,
		MazeRollIncrease,
		MazeYawDecrease,
		MazeYawIncrease,
		KEYBOARD_COUNT,
	};

	class Input {

	public:

		static constexpr int KeyboardValues[KEYBOARD_COUNT] = {
			GLFW_KEY_ESCAPE, // Quit
			GLFW_KEY_A,      // MazeRollDecrease
			GLFW_KEY_D,      // MazeRollIncrease
			GLFW_KEY_W,      // MazeYawDecrease
			GLFW_KEY_S,      // MazeYawIncrease
		};

		Input();

		KeyState getKey(int key) const;
		KeyState getMouseL() const;
		KeyState getMouseR() const;

		void setKey(int key, KeyState value);
		void setMouseL(KeyState value);
		void setMouseR(KeyState value);

		float getMouseX() const;
		float getMouseY() const;
		float getDeltaMouseX() const;
		float getDeltaMouseY() const;
		void  setMousePos(float x, float y);

		float getScrollX() const;
		float getScrollY() const;
		void  setScroll(float x, float y);

	private:
		KeyState keyboard[KEYBOARD_COUNT];
		KeyState mouseL;
		KeyState mouseR;
		float mouseX;
		float mouseY;
		float lastMouseX;
		float lastMouseY;
		float scrollX;
		float scrollY;
	};
}

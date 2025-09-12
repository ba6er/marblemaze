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
		MazeYawDecrease,
		MazeYawIncrease,
		MazePitchDecrease,
		MazePitchIncrease,
		CameraAngleDecrease,
		CameraAngleIncrease,
		KEYBOARD_COUNT,
	};

	class Input {

	public:

		static constexpr int KeyboardValues[KEYBOARD_COUNT] = {
			GLFW_KEY_ESCAPE, // Quit
			GLFW_KEY_A,      // MazeYawDecrease
			GLFW_KEY_D,      // MazeYawIncrease
			GLFW_KEY_S,      // MazePitchDecrease
			GLFW_KEY_W,      // MazePitchIncrease
			GLFW_KEY_Q,      // CameraAngleDecrease
			GLFW_KEY_E,      // CameraAngleIncrease
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
		float scrollX;
		float scrollY;
	};
}

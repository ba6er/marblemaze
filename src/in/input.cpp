#include <in/input.hpp>
#include <util.hpp>

using namespace in;

Input::Input()
		: mouseL(Released), mouseR(Released)
		, mouseX(0), mouseY(0)
		, lastMouseX(0), lastMouseY(0)
		, scrollX(0), scrollY(0) {
	for (int i = 0; i < KeyboardCount; i++) {
		keyboard[i] = Released;
	}
}

KeyState Input::getKey(int key) const {
	if (key >= KeyboardCount) {
		DEBUG_WARNING("Key outside of possible values!");
		return Released;
	}
	return keyboard[key];
}

KeyState Input::getMouseL() const {
	return mouseL;
}

KeyState Input::getMouseR() const {
	return mouseR;
}

void Input::setKey(int key, KeyState value) {
	if (key >= KeyboardCount) {
		DEBUG_WARNING("Key outside of possible values!");
		return;
	}
	keyboard[key] = value;
}

void Input::setMouseL(KeyState value) {
	mouseL = value;
}

void Input::setMouseR(KeyState value)
{
	mouseR = value;
}

float Input::getMouseX() const {
	return mouseX;
}

float Input::getMouseY() const {
	return mouseY;
}

float Input::getDeltaMouseX() const {
	return mouseX - lastMouseX;
}

float Input::getDeltaMouseY() const {
	return mouseY - lastMouseY;
}

void Input::setMousePos(float x, float y) {
	lastMouseX = mouseX;
	lastMouseY = mouseY;
	mouseX = x;
	mouseY = y;
}

float Input::getScrollX() const {
	return scrollX;
}

float Input::getScrollY() const {
	return scrollY;
}

void Input::setScroll(float x, float y) {
	scrollX = x;
	scrollY = y;
}

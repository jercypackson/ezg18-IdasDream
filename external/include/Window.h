#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <functional>
#include <unordered_map>
#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "IDManager.h"

struct MouseInput {
	enum class Action {
		PRESSED, RELEASED, SCROLLED
	} action;

	enum class Button {
		LEFT, RIGHT, MIDDLE
	} button;

	float scrollDelta;
};

struct KeyInput {
	enum class Action {
		PRESSED, RELEASED
	} action;

	enum class Key {
		F1 = GLFW_KEY_F1,
		F2 = GLFW_KEY_F2,
		F3 = GLFW_KEY_F3,
		F4 = GLFW_KEY_F4,
		F5 = GLFW_KEY_F5,
		F6 = GLFW_KEY_F6,
		F7 = GLFW_KEY_F7,
		F8 = GLFW_KEY_F8,
		F9 = GLFW_KEY_F9,
		F10 = GLFW_KEY_F10,
		F11 = GLFW_KEY_F11,
		F12 = GLFW_KEY_F12,
		W = GLFW_KEY_W,
		A = GLFW_KEY_A,
		S = GLFW_KEY_S,
		D = GLFW_KEY_D,
		Q = GLFW_KEY_Q,
		E = GLFW_KEY_E,
		UP = GLFW_KEY_UP,
		DOWN = GLFW_KEY_DOWN,
		LEFT = GLFW_KEY_LEFT,
		RIGHT = GLFW_KEY_RIGHT,
		ESCAPE = GLFW_KEY_ESCAPE,
		ENTER = GLFW_KEY_ENTER,
		TAB = GLFW_KEY_TAB,
		SPACE = GLFW_KEY_SPACE,
		LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT
	} key;
};

struct WindowParams {
	int width = 800;
	int height = 800;
	bool fullscreen = false;
	std::string title = "EzEngine";
	int gl_version_major = 4;
	int gl_version_minor = 3;
	int samples = 1;
};

typedef std::function<void(std::string)> DebugCallback;
typedef std::function<void(const MouseInput&)> MouseInputCallback;
typedef std::function<void(const KeyInput&)> KeyInputCallback;

class Window
{
private:
	WindowParams _params;
	GLFWwindow* _window;

	bool _initialized;

	std::unordered_map<ID, MouseInputCallback> _mouseCBs;
	std::unordered_map<ID, KeyInputCallback> _keyCBs;
	

public:
	Window(WindowParams params);
	~Window();

	int getWidth() const { return _params.width; }
	int getHeight() const { return  _params.height; }
	std::string getTitle() const { return _params.title; }
	double getTime() const { return glfwGetTime(); }
	void getCursorPosition(int& x, int& y) const { 
		double x_, y_;
		glfwGetCursorPos(_window, &x_, &y_); 
		x = static_cast<int>(x_);
		y = static_cast<int>(y_);
	};
	void getCursorPositionOfCenter(int& x, int&y) const {
		x = _params.width / 2;
		y = _params.height / 2;
	}
	void setCursorPosition(int x, int y) const {
		if (x < 0 || x >= _params.width || y < 0 || y >= _params.height) return;
		glfwSetCursorPos(_window, static_cast<double>(x), static_cast<double>(y));
	}
	void setCursorPositionToCenter() const {
		setCursorPosition(_params.width / 2, _params.height / 2);
	}
	void hideCursor() const {
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	void showCursor() const {
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	bool isKeyPressed(KeyInput::Key key) const {
		int state = glfwGetKey(_window, static_cast<int>(key));
		return state == GLFW_PRESS;
	}

	bool open();
	void endFrame();
	bool shouldClose();
	void close();
	void setFPSinTitle(int fps);

	void registerDebugCallback(DebugCallback cb);
	ID registerMouseInputHandler(MouseInputCallback cb);
	ID registerKeyInputHandler(KeyInputCallback cb);
	void unregisterMouseInputHandler(ID id);
	void unregisterKeyInputHandler(ID id);
	
	void mouseInputEvent(const MouseInput& inp);
	void keyInputEvent(const KeyInput& inp);

	void bindDefaultFramebuffer() const;
};


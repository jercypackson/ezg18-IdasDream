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
		ESCAPE = GLFW_KEY_ESCAPE,
		ENTER = GLFW_KEY_ENTER,
		SPACE = GLFW_KEY_SPACE
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

	int getWidth() { return _params.width; }
	int getHeight() { return  _params.height; }
	std::string getTitle() { return _params.title; }
	double getTime() { return glfwGetTime(); }
	void getCursorPosition(int& x, int& y) { 
		double x_, y_;
		glfwGetCursorPos(_window, &x_, &y_); 
		x = static_cast<int>(x_);
		y = static_cast<int>(y_);
	};

	bool open();
	void endFrame();
	bool shouldClose();
	void close();
	void setFPSinTitle(int fps);

	void registerDebugCallback(DebugCallback cb);
	ID registerMouseInputHandler(MouseInputCallback cb);
	ID registerKeyInputHandler(KeyInputCallback cb);
	
	void mouseInputEvent(const MouseInput& inp);
	void keyInputEvent(const KeyInput& inp);
};


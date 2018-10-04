#pragma once

#include <iostream>
#include <memory>

#include <GL\glew.h>

#include "Window.h"

class Application
{
private:
	bool _started;
	int _frameCounter;
	float _tFPS;

protected:
	Window _window;
	float _tSum;

public:
	Application(WindowParams params);
	virtual ~Application();

	bool start();
	void stop();

	virtual void init() = 0;
	virtual void update(float dt) = 0;
	virtual void render(float dt) = 0;
};


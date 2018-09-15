#pragma once

#include <iostream>
#include <memory>

#include <GL\glew.h>

#include "Window.h"
#include "Camera.h"
#include "Shader.h"

class Application
{
private:
	bool _started;

	int _frameCounter;
	float _tFPS;

protected:
	Window _window;
	Camera _camera;

	float _tSum;

	std::unique_ptr<Shader> loadShader(std::string name, std::initializer_list<ShaderType> types = { ShaderType::VERTEX, ShaderType::FRAGMENT }, std::string relPath = "assets/shader/");

public:
	Application(WindowParams params);
	virtual ~Application();

	bool start();
	void stop();

	virtual void init() = 0;
	virtual void render(float dt) = 0;
	virtual void destroy() = 0;
};


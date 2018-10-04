#pragma once

#include "Application.h"
#include "ArcballCamera.h"
#include "Geometry.h"
#include "Shader.h"
#include <glm\glm.hpp>
#include "Light.h"
#include "SceneObject.h"

class IdasDream
	: public Application
{
public:
	IdasDream(int width, int height, bool fullscreen);
	virtual ~IdasDream();

	void init() override;
	void render(float dt) override;
	void update(float dt) override;
	void destroy() override;

private:
	std::shared_ptr<Shader> _shader;
	std::vector<Geometry> _obj;
	ArcballCamera _arcballCamera;

	DirectionalLight _dirL;

	SceneObject* _scene;

	std::unique_ptr<Buffer> _dataBuffer;

};

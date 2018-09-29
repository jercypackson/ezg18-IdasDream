#pragma once

#include "Application.h"
#include "ArcballCamera.h"
#include "Geometry.h"
#include <glm\glm.hpp>

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
	std::unique_ptr<Shader> _shader;
	std::vector<Geometry> _obj;
	ArcballCamera _arcballCamera;
	glm::mat4 _mm;
};

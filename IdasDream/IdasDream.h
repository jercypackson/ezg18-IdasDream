#pragma once

#include "Application.h"
#include "ArcballCamera.h"
#include "Geometry.h"
#include "Shader.h"
#include <glm\glm.hpp>
#include "Light.h"
#include "SceneObject.h"
#include "Hierachy.h"

class IdasDream
	: public Application
{
public:
	IdasDream(int width, int height, bool fullscreen, float timeOffset = 0, float speed = 1);
	virtual ~IdasDream();

	void init() override;
	void update(float dt) override;
	void animate(float dt);
	void render(float dt) override;

private:
	std::shared_ptr<Shader> _shader;
	std::vector<Geometry> _obj;
	ArcballCamera _arcballCamera;

	std::unique_ptr<Hierachy> _hierachy;

	std::unique_ptr<Buffer> _vertDataBuffer;
	std::unique_ptr<Buffer> _fragDataBuffer;

	float getTime();
	float _timeOffset;
	float _speed;

	std::vector<float> _idaTime;
	std::vector<Transformation> _idaTransf;
	SceneObject* _ida;

	std::vector<VertData> _vertData;

};

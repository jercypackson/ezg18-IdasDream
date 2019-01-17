#pragma once

#include "Application.h"
#include "FlyCamera.h"
#include "StaticMesh.h"
#include "Shader.h"
#include <glm\glm.hpp>
#include "Light.h"
#include "SceneObject.h"
#include "Hierachy.h"
#include "AnimatedCamera.h"
#include "OrthoCamera.h"
#include "Particles.h"
#include <irrKlang\irrKlang.h>

class IdasDream
	: public Application
{
public:
	IdasDream(int width, int height, bool fullscreen, int samples);
	virtual ~IdasDream();

	void init() override;
	void update(float dt) override;
	void animate(float time) const;
	void render(float dt) override;

private:
	irrklang::ISoundEngine* soundEngine;
	irrklang::ISound* sound;

	Particles* _particles;

	std::shared_ptr<Shader> _shader;
	std::vector<StaticMesh> _obj;

	OrthoCamera* _camera = new OrthoCameraAnimated({ -0.8f, 0.8f, -0.45f, 0.45f, 0.1f, 100.f });
	bool _animatedCamera = true;


	std::unique_ptr<Buffer> _vertDataBuffer;
	std::unique_ptr<Buffer> _fragDataBuffer;
	std::unique_ptr<Buffer> _bonesBuffer;
	std::unique_ptr<Buffer> _boneDataBuffer;
	std::unique_ptr<Buffer> _boneDataStartBuffer;

	bool _pause = false;
	bool _nextFrame = false;
	float _time;
	float _timeOffset = 0;
	float _speed = 1;
	float _ticksPerSecond;

	std::vector<VertData> _vertData;
	std::vector<glm::mat4> _bones;

	Animation _camAnim;
	Animation _idaAnim;

	SceneObject* _root;

	void reload();
};

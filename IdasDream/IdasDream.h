#pragma once

#include "Application.h"
#include "FlyCamera.h"
#include "ArcballCamera.h"
#include "StaticMesh.h"
#include "Shader.h"
#include <glm\glm.hpp>
#include "Light.h"
#include "SceneObject.h"
#include "Hierachy.h"
#include "AnimatedCamera.h"
#include "OrthoCamera.h"
#include "Particles.h"

class IdasDream
	: public Application
{
public:
	IdasDream(int width, int height, bool fullscreen, int samples);
	virtual ~IdasDream();

	void init() override;
	void update(float dt) override;
	void animate(float dt);
	void render(float dt) override;

private:
	Particles* _particles;

	std::shared_ptr<Shader> _shader;
	std::vector<StaticMesh> _obj;





	//_arcballCamera({ 60.0f, width / (float)height, 0.1f, 100.0f }),
//_animatedCamera({ 60.0f, width / (float)height, 0.1f, 100.0f })


	ArcballCamera _arcballCamera = ArcballCamera({ 60.0f, 16.0f/9.0f, 0.1f, 100.0f });
	//AnimatedCamera _animatedCamera;
	OrthoCamera _oc = OrthoCamera({ -48, 48, -27, 27, -90, 90 });
	FlyCamera _flyCam = FlyCamera({ -48, 48, -27, 27, -90, 90 });

	Camera* _camera;





	std::unique_ptr<Buffer> _vertDataBuffer;
	std::unique_ptr<Buffer> _fragDataBuffer;
	std::unique_ptr<Buffer> _bonesBuffer;
	std::unique_ptr<Buffer> _boneDataBuffer;
	std::unique_ptr<Buffer> _boneDataStartBuffer;

	bool _pause = false;
	float _time;
	float _timeOffset = 0;
	float _speed = 1;
	float _ticksPerSecond;
	bool _useArcballCam;

	std::vector<VertData> _vertData;
	std::vector<glm::mat4> _bones;

	Animation _camAnim;
	Animation _idaAnim;

	SceneObject* _root;

	void reload();
};

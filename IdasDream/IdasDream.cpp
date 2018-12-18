#include "pch.h"
#include "IdasDream.h"
#include <glm/glm.hpp>

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

#include "StaticMesh.h"
#include "Extensions.h"

#include "ShaderManager.h"

#include "Importer.h"

#include <limits>

#include "Data.h"
#include "Transform.h"
#include <glm/gtc/constants.hpp>

#include "Bones.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>

#include <nlohmann/json.hpp>
// for convenience
using json = nlohmann::json;

// ifstream constructor.
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

#include <INIReader.h>

#include "Particles.h"

IdasDream::IdasDream(int width, int height, bool fullscreen, int samples = 1)
	: Application({ width, height, fullscreen, "Ida's Dream", 4, 6, samples }),
	_arcballCamera({ 60.0f, width / (float)height, 0.1f, 100.0f }),
	_animatedCamera({ 60.0f, width / (float)height, 0.1f, 100.0f })
{
	_arcballCamera.setZoom(50);
}

IdasDream::~IdasDream()
{
}

void IdasDream::init()
{

	_window.registerKeyInputHandler([&cto = _currTimeOffset, &to = _timeOffset, &w = _window](const KeyInput& inp) {
		if (inp.action != KeyInput::Action::RELEASED) return;

		switch (inp.key) {
		case KeyInput::Key::F1:
			cto = static_cast<float>(w.getTime()) - to;
			break;
		}
	});

	_window.registerKeyInputHandler([this](const KeyInput& inp) {
		if (inp.action != KeyInput::Action::RELEASED) return;

		switch (inp.key) {
		case KeyInput::Key::F2:
			reload();
			break;
		}
	});


	_arcballCamera.registerToWindow(_window);
	_animatedCamera.registerToWindow(_window);

	_root = Importer(Extensions::assets + "objects").import();
	
	// load and use default shader
	_shader = ShaderManager::getShader("phongPhong");
	_shader->registerToWindow(_window);
	_shader->use();

	//todo reserve
	std::vector<GeometryData> geometryData;
	std::vector<BoneData> boneData;
	std::vector<int> boneDataStart;
	std::vector<FragData> fragData;

	_bones.resize(Bones::size());

	Hierachy::forEach(_root, [&gd = geometryData, &vd = _vertData, &fd = fragData, &b = _bones, &bd = boneData, &bds = boneDataStart](SceneObject* s) {
		if (s->getHasData()) {

			gd.push_back(s->getGeometryData());

			vd.push_back({
				s->getModelMatrix(),
				s->getNormalMatrix()
			});

			s->getMaterial()->setFragmentData(fd);

			s->setBoneData(bd, bds);
		}
		
		//do for the armature
		s->setBones(b);
	});

	//normalize vertex weigths
	for (size_t i = 0; i < boneData.size(); i++)
	{
		float sum = 0;
		for (size_t j = 0; j < NUM_BONES_PER_VEREX; j++)
		{
			sum += boneData[i].weight[j];
		}
		
		if (sum != 1) {
			for (size_t j = 0; j < NUM_BONES_PER_VEREX; j++)
			{
				boneData[i].weight[j] /= sum;
			}
		}
	}

	DrawCallInfo dci = DrawCallInfo::fromGeometryData(geometryData);
	_obj.push_back(StaticMesh(dci, geometryData));

	_vertDataBuffer = std::make_unique<Buffer>(&_vertData[0], sizeof(VertData) * _vertData.size(), BufferUsage::DYNAMIC);
	_vertDataBuffer->bind(BufferType::SSBO, 0);

	_bonesBuffer = std::make_unique<Buffer>(&_bones[0], sizeof(glm::mat4) * _bones.size(), BufferUsage::DYNAMIC);
	_bonesBuffer->bind(BufferType::SSBO, 1);
	
	_boneDataBuffer = std::make_unique<Buffer>(&boneData[0], sizeof(BoneData) * boneData.size(), BufferUsage::STATIC);
	_boneDataBuffer->bind(BufferType::SSBO, 2);
	
	_boneDataStartBuffer = std::make_unique<Buffer>(&boneDataStart[0], sizeof(int) * boneDataStart.size(), BufferUsage::STATIC);
	_boneDataStartBuffer->bind(BufferType::SSBO, 3);
	
	_fragDataBuffer = std::make_unique<Buffer>(&fragData[0], sizeof(FragData) * fragData.size(), BufferUsage::STATIC);
	_fragDataBuffer->bind(BufferType::SSBO, 4);


	// Initialize lights
	DirectionalLight dirL = DirectionalLight(glm::vec3(1.0f), glm::vec3(0.5,-1,-0.3));

	_shader->setUniform("dirL.color", dirL.color);
	_shader->setUniform("dirL.direction", dirL.direction);


	// some OpenGL defaults
	glClearColor(0.33f,0.4f,0.5f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);



	//init animation


	std::map<float, Transform> cam = {
		{  0.0f, Transform(glm::vec3(-33.4545f, 32.3217f, 18.7543f), -glm::vec3(0.710f,1.165f,0)) },
		{ 10.0f, Transform(glm::vec3(-33.7117f, 21.4750f, 32.1298f), -glm::vec3(0.380f,0.845f,0)) },
		{ 20.0f, Transform(glm::vec3(-18.1747f, 35.7631f, 33.7641f), -glm::vec3(0.755f,0.455f,0)) },
	};
	_camAnim = Animation(cam);



	// particles
	_particles = new Particles();




	reload();

	//update offset
	_currTimeOffset += static_cast<float>(_window.getTime()) - _timeOffset;
}

void IdasDream::update(float dt)
{
	_arcballCamera.update(_window, dt);

	float time = getTime();

	animate(time);

	_vertData.clear();
	_bones.clear();
	_bones.resize(Bones::size());

	Hierachy::forEach(_root, [&vd = _vertData, &b = _bones, &t = time](SceneObject* s) {
		if (s->getHasData()) {

			VertData data = {
				s->getModelMatrix(),
				s->getNormalMatrix()
			};

			if (s->getName().find("Twist") != std::string::npos) {

				float twist = t * 0.1f;

				data.twistParam = std::min(t * 0.1f, glm::pi<float>() / 16.0f);
			}

			vd.push_back(data);
		}

		s->setBones(b);
	});
	_shader->use();
	_vertDataBuffer->update(&_vertData[0], sizeof(VertData) * _vertData.size());
	_bonesBuffer->update(&_bones[0], sizeof(glm::mat4) * _bones.size());

	auto camt = _camAnim.getCurrentTransform(time);
	if (camt) {
		_animatedCamera.update(camt.value());
	}

	//_particles->compute(dt);
}

void IdasDream::animate(float time)
{
	Hierachy::forEach(_root, [&t = time](SceneObject* s) {
		s->animate(t);
	});
}

float IdasDream::getTime()
{
	return (static_cast<float>(_window.getTime()) - _currTimeOffset) * _speed;
}


void IdasDream::render(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_shader->use();

	_vertDataBuffer->bind(BufferType::SSBO, 0);
	_bonesBuffer->bind(BufferType::SSBO, 1);
	_boneDataBuffer->bind(BufferType::SSBO, 2);
	_boneDataStartBuffer->bind(BufferType::SSBO, 3);
	_fragDataBuffer->bind(BufferType::SSBO, 4);


	if (_useArcballCam) {
		_shader->setUniform("viewProjMatrix", _arcballCamera.getViewProjectionMatrix());
		_shader->setUniform("camera_world", _arcballCamera.getPosition());
	}
	else {
		_shader->setUniform("viewProjMatrix", _animatedCamera.getViewProjectionMatrix());
		_shader->setUniform("camera_world", _animatedCamera.getPosition());
	}

#if writ
	bool write = false;

	if (write) {
		auto pos = _arcballCamera.getPosition();
		auto rot = -_arcballCamera.getRot();
		std::cout << "glm::vec3("
			<< pos.x << "," << pos.y << "," << pos.z << "), glm::vec3("
			<< rot.x << "," << rot.y << "," << rot.z << ")"
			<< std::endl;
	}
#endif

	_obj[0].bindVertexArray();
	_obj[0].draw();

	_shader->unuse();

	glEnable(GL_PROGRAM_POINT_SIZE);
	_particles->draw(_useArcballCam ? _arcballCamera.getViewProjectionMatrix() : _animatedCamera.getViewProjectionMatrix());
	glDisable(GL_PROGRAM_POINT_SIZE);
	
}

void IdasDream::reload()
{
	INIReader reader(Extensions::assets + "settings.ini");

	_speed = static_cast<float>(reader.GetReal("Animation", "speed", 1));
	_timeOffset = static_cast<float>(reader.GetReal("Animation", "timeOffset", 0)) / _speed;
	_ticksPerSecond = static_cast<float>(reader.GetReal("Animation", "ticksPerSecond", 24));

	_useArcballCam = reader.GetBoolean("Camera", "arcball", false);

	auto animationPath = reader.Get("Animation", "animationPath", "animation.json");

	std::ifstream ifs(Extensions::assets + animationPath, std::ifstream::in);
	json root;
	ifs >> root;
	ifs.close();

	for (json::iterator it = root.begin(); it != root.end(); ++it) {
		std::string name = it.key();

		auto sceneObj = Hierachy::find(_root, name);

		auto obj = it.value();

		{
			auto oa = obj["ObjectAnimation"];
			if (!oa.is_null()) {

				std::vector<float> time;
				std::vector<Transform> transform;

				auto s = oa.size();
				time.reserve(s);
				transform.reserve(s);

				for (auto& a : oa) {

					time.push_back((float)(a[0]) / _ticksPerSecond);

					auto p = a[1];
					auto r = a[2];
					transform.push_back(Transform(glm::vec3(p[0], p[1], p[2]), glm::radians(glm::vec3(r[0], r[1], r[2]))));
				}

				sceneObj->setAnimation(Animation(time, transform));
			}
		}
		{
			auto ra = obj["RiggedAnimation"];
			if (!ra.is_null()) {

				std::vector<float> time;
				std::vector<string> animation;

				auto s = ra.size();
				time.reserve(s);
				animation.reserve(s);

				for (auto& a : ra) {

					time.push_back((float)(a[0]) / _ticksPerSecond);
					animation.push_back((std::string)(a[1]));
				}

				Hierachy::forEach(sceneObj, [&t = time, &a = animation](SceneObject* s) {
					s->addAnimationSequence(t, a);
				});
			}
		}

		bool test = true;
	}

}

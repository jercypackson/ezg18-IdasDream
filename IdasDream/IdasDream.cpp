#include "pch.h"
#include "IdasDream.h"
#include <glm/glm.hpp>

#include "StaticMesh.h"
#include "Extensions.h"

#include "ShaderManager.h"

#include "Importer.h"

#include "Data.h"
#include "Transform.h"

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
#include <cstdlib>


IdasDream::IdasDream(const int width, const int height, const bool fullscreen, const int samples = 1)
	: Application({ width, height, fullscreen, "Ida's Dream", 4, 6, samples })
{
}

IdasDream::~IdasDream()
{
	if (sound) sound->drop();
	soundEngine->drop();
}

void IdasDream::init()
{
	_window.registerKeyInputHandler(
		[&t = _time, &to = _timeOffset, &p = _pause, &n = _nextFrame, &tps = _ticksPerSecond, &particles = _particles, &
		s = sound, &ac = _animatedCamera, &c = _camera, &w = _window](const KeyInput& inp)
	{
		if (inp.action != KeyInput::Action::RELEASED) return;

		switch (static_cast<int>(inp.key))
		{
			case static_cast<int>(KeyInput::Key::F1) :
				t = to;
				particles->clear();
				p = false;
				if (s)
				{
					s->setPlayPosition(0);
					s->setIsPaused(p);
				}
				break;
			case GLFW_KEY_P:
				p = !p;
				if (s) s->setIsPaused(p);
				break;
			case GLFW_KEY_N:
				n = true;
				break;
			case GLFW_KEY_T:
				std::cout << "Time: " << t << ", Ticks: " << t * tps << std::endl;
				break;
			case GLFW_KEY_C:
				ac = !ac;
				if (ac) w.hideCursor();
				else w.showCursor();
				break;
			case GLFW_KEY_X:
				c->printPose();
				break;
		}
	});

	_window.registerKeyInputHandler([this](const KeyInput& inp)
	{
		if (inp.action != KeyInput::Action::RELEASED) return;

		switch (inp.key)
		{
		case KeyInput::Key::F2:
			reload();
			break;
		}
	});


	_camera->registerToWindow(_window);

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

	Hierachy::forEach(
		_root,
		[&gd = geometryData, &vd = _vertData, &fd = fragData, &b = _bones, &bd = boneData, &bds = boneDataStart](
			SceneObject* s)
	{
		if (s->getHasData())
		{
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

	//normalize vertex weights
	for (size_t i = 0; i < boneData.size(); i++)
	{
		float sum = 0;
		for (size_t j = 0; j < NUM_BONES_PER_VEREX; j++)
		{
			sum += boneData[i].weight[j];
		}

		if (sum != 1)
		{
			for (size_t j = 0; j < NUM_BONES_PER_VEREX; j++)
			{
				boneData[i].weight[j] /= sum;
			}
		}
	}

	DrawCallInfo dci = DrawCallInfo::fromGeometryData(geometryData);
	_obj.push_back(StaticMesh(dci, geometryData));

	_vertDataBuffer = std::make_unique<Buffer
	>(&_vertData[0], sizeof(VertData) * _vertData.size(), BufferUsage::DYNAMIC);
	_vertDataBuffer->bind(BufferType::SSBO, 0);

	_bonesBuffer = std::make_unique<Buffer>(&_bones[0], sizeof(glm::mat4) * _bones.size(), BufferUsage::DYNAMIC);
	_bonesBuffer->bind(BufferType::SSBO, 1);

	_boneDataBuffer = std::make_unique<Buffer>(&boneData[0], sizeof(BoneData) * boneData.size(), BufferUsage::STATIC);
	_boneDataBuffer->bind(BufferType::SSBO, 2);

	_boneDataStartBuffer = std::make_unique<Buffer>(&boneDataStart[0], sizeof(int) * boneDataStart.size(),
		BufferUsage::STATIC);
	_boneDataStartBuffer->bind(BufferType::SSBO, 3);

	_fragDataBuffer = std::make_unique<Buffer>(&fragData[0], sizeof(FragData) * fragData.size(), BufferUsage::STATIC);
	_fragDataBuffer->bind(BufferType::SSBO, 4);


	// Initialize lights
	DirectionalLight dirL = DirectionalLight(glm::vec3(1.0f), glm::vec3(0.5, -1, -0.3));

	_shader->setUniform("dirL.color", dirL.color);
	_shader->setUniform("dirL.direction", dirL.direction);


	// some OpenGL defaults
	glClearColor(0.23f, 0.3f, 0.4f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);


	// particles
	_particles = new Particles();


	//music yay
	// start the sound engine with default parameters
	soundEngine = irrklang::createIrrKlangDevice();
	if (!soundEngine)
	{
		printf("Could not startup irrklang Sound Engine\n");
	}
	auto path = Extensions::assets + "music/Creepy-doll-music.mp3";
	sound = soundEngine->play2D(path.c_str(), true, true, true);
	system("CLS"); //clear bc this output isn't needed

	reload();

	if (_animatedCamera) _window.hideCursor();
	else _window.showCursor();

	_time = _timeOffset; // reset time

	if (sound) sound->setIsPaused(false);
}

void IdasDream::update(float dt)
{
	_camera->update(_window, dt);

	if (_pause && !_nextFrame) return;
	_nextFrame = false;

	if (dt > 1 / 30.f)
	{
		//on debugging to avoid big jumps
		dt = 1 / 30.f;
	}

	_time += dt * _speed;

	// music
	const float start = 226.611f;
	const float end = 5540.f / 24.f;
	if (_time > start && _time < end)
	{
		sound->setVolume((_time - start) * -1 / (end - start) + 1);
	}
	else if (_time > end + 1 && sound && sound->getVolume() < 0.5f)
	{
		auto pathStillAlive = Extensions::assets + "music/12 Still Alive.mp3";
		sound = soundEngine->play2D(pathStillAlive.c_str(), false, false, true);
	}
	else if (sound->isFinished())
	{
		_window.close();
	}

	//

	animate(_time);

	_vertData.clear();
	_bones.clear();
	_bones.resize(Bones::size());

	Hierachy::forEach(_root, [&vd = _vertData, &b = _bones, &t = _time](SceneObject* s)
	{
		if (s->getHasData())
		{
			VertData data = {
				s->getModelMatrix(),
				s->getNormalMatrix()
			};

			const float start1 = 2951.92f / 24.f;
			const float end1 = start1 + glm::pi<float>() / 16.0f * 10;
			const float start2 = 3390.f / 24.f;
			const float end2 = 3490.f / 24.f;


			if (s->getName().find("Twist.000") != std::string::npos)
			{
				if (t > start1)
				{
					if (t < end1)
					{
						data.twistParam = -(t - start1) * 0.1f;
					}
					else if (t < start2)
					{
						data.twistParam = -glm::pi<float>() / 16.0f;
					}
					else if (t < end2)
					{
						data.twistParam = (t - start2) * (glm::pi<float>() / 16.0f) / (end2 - start2) - glm::pi<float>()
							/ 16.0f;
					}
					else
					{
						data.twistParam = 0;
					}
				}
			}

			if (s->getName().find("Twist.001") != std::string::npos)
			{
				if (t > start2)
				{
					if (t < end2)
					{
						data.twistParam = (t - start2) * (glm::pi<float>() / 16.0f) / (end2 - start2);
					}
					else
					{
						data.twistParam = glm::pi<float>() / 16.0f;
					}
				}
			}

			if (s->getName().find("Flag") != std::string::npos)
			{
				data.waveParam = glm::mod(t, glm::two_pi<float>()) * 2;
			}

			vd.push_back(data);
		}

		s->setBones(b);
	});
	_shader->use();
	_vertDataBuffer->update(&_vertData[0], sizeof(VertData) * _vertData.size());
	_bonesBuffer->update(&_bones[0], sizeof(glm::mat4) * _bones.size());

	auto camTr = _camAnim.getCurrentTransform(_time, true);
	if (_animatedCamera && camTr)
	{
		_camera->update(camTr.value(), _animatedCamera);
	}

	_particles->compute(dt, _time, _idaAnim.getCurrentTransform(_time).value_or(Transform(glm::vec3())));
}

void IdasDream::animate(float time) const
{
	Hierachy::forEach(_root, [&t = time](SceneObject* s)
	{
		s->animate(t);
	});
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


	_shader->setUniform("viewProjMatrix", _camera->getViewProjectionMatrix());
	_shader->setUniform("camera_world", _camera->getPosition());

	auto idaTr = _idaAnim.getCurrentTransform(_time);
	_shader->setUniform("feetPosition", idaTr->pos + idaTr->quat * glm::vec3(0.0f, 0.0f, -1.0f));

	_obj[0].bindVertexArray();
	_obj[0].draw();

	_shader->unuse();

	_particles->render(_camera->getViewProjectionMatrix());
}

void IdasDream::reload()
{
	INIReader reader(Extensions::assets + "settings.ini");

	_speed = static_cast<float>(reader.GetReal("Animation", "speed", 1));
	_timeOffset = static_cast<float>(reader.GetReal("Animation", "timeOffset", 0));
	_ticksPerSecond = static_cast<float>(reader.GetReal("Animation", "ticksPerSecond", 24));

	auto animationPath = reader.Get("Animation", "animationPath", "animation.json");

	std::ifstream ifs(Extensions::assets + animationPath, std::ifstream::in);
	json root;
	ifs >> root;
	ifs.close();

	for (json::iterator it = root.begin(); it != root.end(); ++it)
	{
		std::string name = it.key();

		auto sceneObj = Hierachy::find(_root, name);
		if (!sceneObj) continue;

		auto obj = it.value();

		{
			auto oa = obj["ObjectAnimation"];
			if (!oa.is_null())
			{
				std::vector<float> time;
				std::vector<Transform> transform;

				auto s = oa.size();
				time.reserve(s);
				transform.reserve(s);

				for (auto& a : oa)
				{
					time.push_back(static_cast<float>(a[0]) / _ticksPerSecond);

					auto p = a[1];
					auto r = a[2];

					auto s = a[3];

					transform.push_back(Transform(
						glm::vec3(p[0], p[1], p[2]),
						radians(glm::vec3(r[0], r[1], r[2])),
						s.empty() ? 1 : float(s)
					));
				}

				Animation animation = Animation(time, transform);

				//this is getting messy, todo: find a better way?
				if (name.find("Ida") != string::npos)
				{
					_idaAnim = animation;
				}
				else if (name.find("Camera") != string::npos)
				{
					_camAnim = animation;
				}

				sceneObj->setAnimation(animation);
			}
		}
		{
			auto ra = obj["RiggedAnimation"];
			if (!ra.is_null())
			{
				std::vector<float> time;
				std::vector<string> animation;

				auto s = ra.size();
				time.reserve(s);
				animation.reserve(s);

				for (auto& a : ra)
				{
					time.push_back(static_cast<float>(a[0]) / _ticksPerSecond);
					animation.push_back(static_cast<std::string>(a[1]));
				}

				Hierachy::forEach(sceneObj, [&t = time, &a = animation](SceneObject* sc)
				{
					sc->addAnimationSequence(t, a);
				});
			}
		}
	}
}

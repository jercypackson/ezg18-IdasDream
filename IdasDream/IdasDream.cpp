#include "pch.h"
#include "IdasDream.h"
#include <glm/glm.hpp>

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

#include "Geometry.h"
#include "Extensions.h"

#include "ShaderManager.h"

#include "Importer.h"

#include <limits>

#include "Data.h"
#include "Transform.h"
#include <glm/gtc/constants.hpp>

IdasDream::IdasDream(int width, int height, bool fullscreen, float timeOffset, float speed)
	: Application({ width, height, fullscreen, "Idas Dream", 4, 6 }),
	_arcballCamera({ 60.0f, width / (float)height, 0.1f, 100.0f }),
	_animatedCamera({ 60.0f, width / (float)height, 0.1f, 100.0f }),
	_timeOffset(-timeOffset), _speed(speed)
{
	_arcballCamera.setZoom(50);
}

IdasDream::~IdasDream()
{
}

void IdasDream::init()
{

	_window.registerKeyInputHandler([&to = _timeOffset, &w = _window](const KeyInput& inp) {
		if (inp.action != KeyInput::Action::RELEASED) return;

		switch (inp.key) {
		case KeyInput::Key::F1:
			to = static_cast<float>(w.getTime());
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

	std::vector<GeometryData> geometryData;
	std::vector<FragData> fragData;

	Hierachy::forEach(_root, [&gd = geometryData, &vd = _vertData, &fd = fragData](SceneObject* s) {
		if (s->getHasData()) {
			gd.push_back(s->getGeometryData());

			vd.push_back({
				s->getModelMatrix(),
				s->getNormalMatrix()
				});

			s->getMaterial()->setFragmentData(fd);
		}
	});

	DrawCallInfo dci = DrawCallInfo::fromGeometryData(geometryData);
	_obj.push_back(Geometry(dci, geometryData));

	_vertDataBuffer = std::make_unique<Buffer>(&_vertData[0], sizeof(VertData) * _vertData.size(), BufferUsage::DYNAMIC);
	_vertDataBuffer->bind(BufferType::SSBO, 0);

	_fragDataBuffer = std::make_unique<Buffer>(&fragData[0], sizeof(FragData) * fragData.size(), BufferUsage::STATIC);
	_fragDataBuffer->bind(BufferType::SSBO, 1);


	// Initialize lights
	DirectionalLight dirL = DirectionalLight(glm::vec3(1.0f), glm::vec3(0.5,-1,-0.3));

	_shader->setUniform("dirL.color", dirL.color);
	_shader->setUniform("dirL.direction", dirL.direction);


	// some OpenGL defaults
	glClearColor(0.33f,0.4f,0.5f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);





	//init animation
	_ida = Hierachy::find(_root, "ida");

	std::map<float, Transform> ida = {
		{   0.0f,	Transform(	glm::vec3(-21, 9, -1),		glm::vec3(	glm::half_pi<float>(),	glm::pi<float>(),		glm::pi<float>())	)	},
		{   3.0f,	Transform(	glm::vec3(-21, 9,  1),		glm::vec3(	glm::half_pi<float>(),	glm::pi<float>(),		glm::pi<float>())	)	},
		{   5.0f,	Transform(	glm::vec3(-21, 9,  6),		glm::vec3(	glm::half_pi<float>(),	glm::pi<float>(),		glm::pi<float>())	)	},
		{   7.33f,	Transform(	glm::vec3(-21, 5, 10.33f),	glm::vec3(	glm::half_pi<float>(),	glm::pi<float>(),		glm::pi<float>())	)	},
		{   8.0f,	Transform(	glm::vec3(-21, 5, 11),		glm::vec3(	glm::half_pi<float>(),	glm::pi<float>(),		glm::pi<float>())	)	},
		{   9.0f,	Transform(	glm::vec3(-21, 5, 11),		glm::vec3(	glm::half_pi<float>(),	glm::half_pi<float>(),	glm::pi<float>())	)	},
		{  11.0f,	Transform(	glm::vec3(-19, 5, 11),		glm::vec3(	glm::half_pi<float>(),	glm::half_pi<float>(),	glm::pi<float>())	)	},
		{  12.0f,	Transform(	glm::vec3(-19, 5, 11),		glm::vec3(	glm::half_pi<float>(),	0,						glm::pi<float>())	)	},
		{  14.0f,	Transform(	glm::vec3(-19, 5,  7),		glm::vec3(	glm::half_pi<float>(),	0,						glm::pi<float>())	)	},
	};

	std::map<float, Transform> cam = {
		{  0.0f, Transform(glm::vec3(-33.4545f, 32.3217f, 18.7543f), -glm::vec3(0.710f,1.165f,0)) },
		{ 10.0f, Transform(glm::vec3(-33.7117f, 21.4750f, 32.1298f), -glm::vec3(0.380f,0.845f,0)) },
		{ 20.0f, Transform(glm::vec3(-18.1747f, 35.7631f, 33.7641f), -glm::vec3(0.755f,0.455f,0)) },
	};


	_camAnim = Animation(cam);

	_ida->setAnimation(Animation(ida));






	//bones
	std::vector<glm::mat4> bones;
	Hierachy::forEach(_root, [](SceneObject* s) {

	});





	//update offset
	_timeOffset += static_cast<float>(_window.getTime());
}

void IdasDream::update(float dt)
{
	_arcballCamera.update(_window, dt);

	float time = getTime();

	animate(time);

	
	//todo: only update if something changed
	_vertData.clear();
	Hierachy::forEach(_root, [&vd = _vertData](SceneObject* s) {
		if (s->getHasData()) {
			vd.push_back({
				s->getModelMatrix(),
				s->getNormalMatrix()
			});
		}
	});
	_vertDataBuffer->update(&_vertData[0], sizeof(VertData) * _vertData.size());

	auto camt = _camAnim.getCurrentTransform(time);
	if (camt) {
		_animatedCamera.update(camt.value());
	}
}

void IdasDream::animate(float time)
{
	Hierachy::forEach(_root, [&t = time](SceneObject* s) {
		s->animate(t);
	});
}

float IdasDream::getTime()
{
	return (static_cast<float>(_window.getTime()) - _timeOffset) * _speed;
}

void IdasDream::render(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//*
	_shader->setUniform("viewProjMatrix", _animatedCamera.getViewProjectionMatrix());
	_shader->setUniform("camera_world", _animatedCamera.getPosition());
	/*/
	_shader->setUniform("viewProjMatrix", _arcballCamera.getViewProjectionMatrix());
	_shader->setUniform("camera_world", _arcballCamera.getPosition());
	//*/

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
}

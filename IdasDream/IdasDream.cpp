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

#include "Bones.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>


IdasDream::IdasDream(int width, int height, bool fullscreen, float timeOffset, float speed)
	: Application({ width, height, fullscreen, "Ida's Dream", 4, 6 }),
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

	//todo reserve
	std::vector<GeometryData> geometryData;
	std::vector<glm::mat4> bones;
	std::vector<BoneData> boneData;
	std::vector<int> boneDataStart;
	std::vector<FragData> fragData;

	bones.resize(Bones::size());

	Hierachy::forEach(_root, [&gd = geometryData, &vd = _vertData, &fd = fragData, &b = bones, &bd = boneData, &bds = boneDataStart](SceneObject* s) {
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


	
	//glm::vec3 Scale, Translation, Skew;
	//glm::quat Orientation;
	//glm::vec4 Perspective;

	//for (size_t i = 0; i < bones.size(); i++)
	//{
	//	glm::decompose(bones[i], Scale, Orientation, Translation, Skew, Perspective);
	//	glm::vec3 Euler = glm::eulerAngles(Orientation);

	//	Scale = Extensions::round(Scale, 4);
	//	Euler = Extensions::round(Euler, 4);
	//	Translation = Extensions::round(Translation, 4);
	//	Skew = Extensions::round(Skew, 4);
	//	Perspective = Extensions::round(Perspective, 4);

	//	bool test = true;
	//}
	


	DrawCallInfo dci = DrawCallInfo::fromGeometryData(geometryData);
	_obj.push_back(Geometry(dci, geometryData));

	_vertDataBuffer = std::make_unique<Buffer>(&_vertData[0], sizeof(VertData) * _vertData.size(), BufferUsage::DYNAMIC);
	_vertDataBuffer->bind(BufferType::SSBO, 0);

	_bonesBuffer = std::make_unique<Buffer>(&bones[0], sizeof(glm::mat4) * bones.size(), BufferUsage::STATIC);
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




	//update offset
	_timeOffset += static_cast<float>(_window.getTime());
}

void IdasDream::update(float dt)
{
	_arcballCamera.update(_window, dt);

	float time = getTime();

	animate(time);

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
	/*
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

#include "pch.h"
#include "Particles.h"
#include <memory>
#include "Extensions.h"
#include "DrawCall.h"

Particles::Particles()
{
	computeShader = std::make_unique<Shader>(Extensions::assets + "shader/particlePhysics", ShaderList{ ShaderType::COMPUTE });
	computeShader->use();
	computeShader->setUniform("MaximumCount", MAX_PARTICLES);

	ssbo_pos[index] = std::make_shared<Buffer>(nullptr, MAX_PARTICLES * sizeof(glm::vec4), BufferUsage::DYNAMIC);
	ssbo_vel[index] = std::make_unique<Buffer>(nullptr, MAX_PARTICLES * sizeof(glm::vec4), BufferUsage::DYNAMIC);

	const int TTL = 10;
	std::vector<glm::vec4> positions;
	std::vector<glm::vec4> velocities;

	positions.push_back(glm::vec4(0, 10, 0, TTL));
	positions.push_back(glm::vec4(0, 5, 0, TTL));
	velocities.push_back(glm::vec4(0, 0, 0, 0));
	velocities.push_back(glm::vec4(0, 0, 0, 0));

	particle_count = (unsigned int)(positions.size());

	index = 0;
	ssbo_pos[index]->update(&positions[0], particle_count);
	ssbo_vel[index]->update(&velocities[0], particle_count);

	index = 1;
	ssbo_pos[index] = std::make_shared<Buffer>(nullptr, MAX_PARTICLES * sizeof(glm::vec4), BufferUsage::DYNAMIC);
	ssbo_vel[index] = std::make_unique<Buffer>(nullptr, MAX_PARTICLES * sizeof(glm::vec4), BufferUsage::DYNAMIC);
	index = 0;



	GLuint value = 0;
	atomicCounter = std::make_unique<Buffer>(&value, sizeof(GLuint), BufferUsage::DYNAMIC);
	atomicCounter->bind(BufferType::ATOMIC_COUNTER, 4);


	//for rendering
	particleShader = std::make_unique<Shader>(Extensions::assets + "shader/particles", ShaderList{ ShaderType::VERTEX, ShaderType::FRAGMENT });

	particleObject[0] = new BaseGeometry(DrawCallInfo{ DrawCallInfo::GeometryType::POINT_ARRAYS, DrawCallInfo::DrawCallType::SINGLE, {0}, {(int)particle_count } });
	particleObject[1] = new BaseGeometry(DrawCallInfo{ DrawCallInfo::GeometryType::POINT_ARRAYS, DrawCallInfo::DrawCallType::SINGLE, {0}, {(int)particle_count } });

	particleObject[0]->attachVertexAttrib(*ssbo_pos[0], VertexAttribFormat{ 4, VertexAttribFormat::DataType::FLOAT });
	particleObject[1]->attachVertexAttrib(*ssbo_pos[1], VertexAttribFormat{ 4, VertexAttribFormat::DataType::FLOAT });
}

void Particles::compute(float delta) {
	computeShader->use();
	computeShader->setUniform("LastCount", particle_count);
	computeShader->setUniform("DeltaT", delta);

	//read
	ssbo_pos[index]->bind(BufferType::SSBO, 0);
	ssbo_vel[index]->bind(BufferType::SSBO, 1);

	index = !index;

	//write
	ssbo_pos[index]->bind(BufferType::SSBO, 2);
	ssbo_vel[index]->bind(BufferType::SSBO, 3);

	GLuint groups = (particle_count / 128) + 1;
	glDispatchCompute(groups, 1, 1);

	glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);

	atomicCounter->bind(BufferType::ATOMIC_COUNTER, 4);

	GLuint cv = 200;
	GLuint* counterValue = &cv;
	atomicCounter->read(counterValue, sizeof(GLuint));

	particle_count = counterValue[0];
	counterValue[0] = 0;

	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
}

void Particles::draw(glm::mat4 viewMatrix) {
	enableBlendMode();

	particleShader->use();

	particleShader->setUniform("modelMatrix", glm::mat4());
	particleShader->setUniform("viewMatrix", viewMatrix);

	particleShader->setUniform("diffuseColor", glm::vec3(1, 0, 0));

	particleObject[index]->draw();

	disableBlendMode();
	particleShader->unuse();
}

void Particles::enableBlendMode() {
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);

	//todo: set once?
	glBlendFunc(GL_SRC_COLOR, GL_SRC_COLOR);
	glBlendEquation(GL_MAX);
}

void Particles::disableBlendMode() {
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}


Particles::~Particles()
{
	for (auto bg : particleObject) {
		delete bg;
	}
}
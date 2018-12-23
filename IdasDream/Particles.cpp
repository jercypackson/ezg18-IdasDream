#include "pch.h"
#include "Particles.h"
#include <memory>
#include "Extensions.h"
#include "DrawCall.h"

Particles::Particles()
{
	//computeShader = std::make_unique<Shader>(Extensions::assets + "shader/particles", ShaderList{ ShaderType::COMPUTE });
	//computeShader->use();
	//computeShader->setUniform("MaximumCount", MAX_PARTICLES);

	//ssbo_vel.push_back(Buffer(nullptr, MAX_PARTICLES * sizeof(glm::vec4), BufferUsage::DYNAMIC));
	//
	//particleObject.push_back(ParticleObject(MAX_PARTICLES));
	//ssbo_vel.push_back(Buffer(nullptr, MAX_PARTICLES * sizeof(glm::vec4), BufferUsage::DYNAMIC));


	//GLuint value = 0;
	//atomicCounter = std::make_unique<Buffer>(&value, sizeof(GLuint), BufferUsage::DYNAMIC);
	//atomicCounter->bind(BufferType::ATOMIC_COUNTER, 4);


	//for rendering
	particleShader = std::make_unique<Shader>(Extensions::assets + "shader/particles", ShaderList{ ShaderType::VERTEX, ShaderType::FRAGMENT });

	particleShader->use();

	particleObject.push_back(ParticleObject(MAX_PARTICLES));

	//const int TTL = 10;
	//std::vector<glm::vec4> positions;
	//std::vector<glm::vec4> velocities;
	//
	//positions.push_back(glm::vec4(0, 100, 0, TTL));
	//positions.push_back(glm::vec4(0, 5, 0, TTL));
	//positions.push_back(glm::vec4(0, 50, 0, TTL));
	//velocities.push_back(glm::vec4(0, 0, 0, 0));
	//velocities.push_back(glm::vec4(0, 0, 0, 0));
	//
	//
	//particle_count = (unsigned int)(positions.size());
	//
	//particleObject[index].getVertexBuffer(0).update(&positions[0], particle_count);
	//ssbo_vel[index].update(&velocities[0], particle_count);

}

void Particles::compute(float delta) {
	/*
	computeShader->use();
	computeShader->setUniform("LastCount", particle_count);
	computeShader->setUniform("DeltaT", delta);
	computeShader->setUniform("MaximumCount", MAX_PARTICLES);

	//read
	particleObject[index].getVertexBuffer(0).bind(BufferType::SSBO, 0);
	ssbo_vel[index].bind(BufferType::SSBO, 1);

	index = !index;

	//write
	particleObject[index].getVertexBuffer(0).bind(BufferType::SSBO, 2);
	ssbo_vel[index].bind(BufferType::SSBO, 3);

	GLuint groups = (particle_count / 128) + 1;
	glDispatchCompute(groups, 1, 1);

	glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);

	atomicCounter->bind(BufferType::ATOMIC_COUNTER, 4);

	GLuint counterValue = 200;
	atomicCounter->read(&counterValue, sizeof(GLuint));

	particle_count = counterValue;
	counterValue = 0; //todo write

	atomicCounter->update(&counterValue, sizeof(GLuint));

	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
	*/
}

void Particles::draw(glm::mat4 viewMatrix) {
	enableBlendMode();

	glEnable(GL_PROGRAM_POINT_SIZE);


	particleShader->use();

	particleShader->setUniform("modelMatrix", glm::mat4(1.0f));
	particleShader->setUniform("viewMatrix", viewMatrix);

	particleShader->setUniform("diffuseColor", glm::vec3(1, 1, 1));

	particleObject[index].bindVertexArray();
	particleObject[index].draw();

	glDisable(GL_PROGRAM_POINT_SIZE);


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
}

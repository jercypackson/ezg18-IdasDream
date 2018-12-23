#include "pch.h"
#include "Particles.h"
#include <memory>
#include "Extensions.h"
#include "DrawCall.h"

Particles::Particles()
{
	computeShader = std::make_unique<Shader>(Extensions::assets + "shader/particles", ShaderList{ ShaderType::COMPUTE });

	//for rendering
	particleShader = std::make_unique<Shader>(Extensions::assets + "shader/particles", ShaderList{ ShaderType::VERTEX, ShaderType::FRAGMENT });

	particleObject.push_back(ParticleObject(particleLocation, MAX_PARTICLES));
	particleObject.push_back(ParticleObject(particleLocation, MAX_PARTICLES));


	// fill buffer

	const int TTL = 3;
	std::vector<glm::vec4> positions;
	positions.push_back(glm::vec4(0.1f,  0.0f, 0.0f, TTL));
	positions.push_back(glm::vec4(0.1f,  5.0f, 5.0f, TTL));
	positions.push_back(glm::vec4(0.1f, 10.0f, 0.0f, TTL));

	std::vector<glm::vec4> velocities;
	velocities.push_back(glm::vec4(0, 0, 0, 0));
	velocities.push_back(glm::vec4(0, 0, 0, 0));
	velocities.push_back(glm::vec4(0, 0, 0, 0));


	particleObject[index].getVertexBuffer(particleLocation).update(&positions[0], positions.size() * sizeof(positions[0]));
	particleObject[index].getVelocitiesBuffer()->update(&velocities[0], velocities.size() * sizeof(velocities[0]));
	
	
	
	particle_count = (unsigned int)positions.size();

	atomicCounter = std::make_unique<Buffer>(&particle_count, sizeof(particle_count), BufferUsage::DYNAMIC);
	atomicCounter->bind(BufferType::ATOMIC_COUNTER, 4); //is this necessary?
}

void Particles::compute(float delta) {
	computeShader->use();
	computeShader->setUniform("LastCount", particle_count);
	computeShader->setUniform("DeltaT", delta);
	computeShader->setUniform("MaximumCount", MAX_PARTICLES); //todo: move to constructor?

	//read
	particleObject[index].getVertexBuffer(particleLocation)	 .bind(BufferType::SSBO, 0);
	particleObject[index].getVelocitiesBuffer()				->bind(BufferType::SSBO, 1);

	index = !index;

	//write
	particleObject[index].getVertexBuffer(particleLocation)	 .bind(BufferType::SSBO, 2);
	particleObject[index].getVelocitiesBuffer()				->bind(BufferType::SSBO, 3);


	GLuint groups = (particle_count / 128) + 1;
	glDispatchCompute(groups, 1, 1);

	glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);

	atomicCounter->bind(BufferType::ATOMIC_COUNTER, 4);

	unsigned int counterValue = 0;
	atomicCounter->read(&counterValue, sizeof(counterValue));

	particle_count = counterValue;
	counterValue = 0;

	atomicCounter->update(&counterValue, sizeof(counterValue));

	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
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

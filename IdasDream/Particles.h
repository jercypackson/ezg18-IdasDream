#pragma once

#include "Shader.h"
#include "Buffer.h"
#include "BaseGeometry.h"
#include "ParticleObject.h"
#include "Transform.h"

class Particles
{
public:
	Particles();
	~Particles();

	void compute(float delta, Transform pose);
	void render(glm::mat4 viewMatrix);

	void enableBlendMode();

	void disableBlendMode();

private:
	std::unique_ptr<Shader> computeShader;
	std::unique_ptr<Shader> particleShader;

	bool index = 0;

	std::vector<ParticleObject> particleObject;
	std::unique_ptr<Buffer> atomicCounter;
	std::vector<GeometryData> gdv;


	unsigned int MAX_PARTICLES = 4096;

	unsigned int particle_count = 0;

	const unsigned int particleLocation = 0;

	double particles_to_spawn = 0;
	const double spawnRatePerSecond = 128;

	glm::vec3 prevPos;
};


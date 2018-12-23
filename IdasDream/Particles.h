#pragma once

#include "Shader.h"
#include "Buffer.h"
#include "BaseGeometry.h"
#include "ParticleObject.h"

class Particles
{
public:
	Particles();
	~Particles();

	void compute(float delta);
	void draw(glm::mat4 viewMatrix);

	void enableBlendMode();

	void disableBlendMode();

private:
	std::unique_ptr<Shader> computeShader;
	std::unique_ptr<Shader> particleShader;

	bool index = 0;

	std::vector<ParticleObject> particleObject;
	std::unique_ptr<Buffer> atomicCounter;
	std::vector<GeometryData> gdv;


	unsigned int MAX_PARTICLES = 256;

	unsigned int particle_count;

	const unsigned int particleLocation = 0;

	double particles_to_spawn = 0;
	const double spawnRatePerSecond = 2;

};


#pragma once

#include "Shader.h"
#include "Buffer.h"
#include "BaseGeometry.h"

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

	std::shared_ptr<Buffer> ssbo_pos[2];
	std::unique_ptr<Buffer> ssbo_vel[2];
	std::unique_ptr<Buffer> atomicCounter;

	unsigned int MAX_PARTICLES = 255;

	unsigned int particle_count;

	BaseGeometry* particleObject[2];
};
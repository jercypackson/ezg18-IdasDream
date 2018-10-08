#pragma once

#include <vector>
#include "Transform.h"
#include <map>
#include <glm\glm.hpp>
#include <optional>

class Animation
{
public:
	Animation();
	Animation(std::vector<float> time, std::vector<Transform> transform);
	Animation(std::map<float, Transform> timeTransformMap);

	std::optional<glm::mat4> getCurrentMatrix(float currTime);

	~Animation();

private:
	std::vector<float> _time;
	std::vector<Transform> _transform;
};


#include "pch.h"
#include "Animation.h"
#include <algorithm>
#include <iostream>

Animation::Animation() { }

Animation::Animation(std::vector<float> time, std::vector<Transform> transform)
	: _time(time), _transform(transform) { }

Animation::Animation(std::map<float, Transform>& timeTransformMap)
{
	for (auto const&[key, val] : timeTransformMap)
	{
		_time.push_back(key);
		_transform.push_back(val);
	}
}

std::optional<glm::mat4> Animation::getCurrentMatrix(float currTime)
{
	auto t = getCurrentTransform(currTime);
	if (t) {
		return t->getMatrix();
	}
	return std::nullopt;
}

float prev = 0;

std::optional<Transform> Animation::getCurrentTransform(float currTime, bool fadeInOut)
{
	if (_time.empty() || currTime < _time.front() || currTime > _time.back()) return std::nullopt;

	auto lessOrEqual = std::prev(std::upper_bound(_time.begin(), _time.end(), currTime), 1);
	auto greaterOrEqual = std::lower_bound(_time.begin(), _time.end(), currTime);

	float delta = (currTime - (*lessOrEqual)) / ((*greaterOrEqual) - (*lessOrEqual));

	if (fadeInOut) {

		//todo: find the point where b * (1 - cos(pi*(c*x)))=y and y=x+a meet

		if (delta < 0.2f) {
			delta = 0.2 - 0.2 * glm::cos((5.0 * glm::pi<double>() * (double)delta) / 2.0);
		}
		else if (delta > 0.8f) {
			delta = 0.8 + 0.2 * glm::sin((5.0 * glm::pi<double>() * (double)delta) / 2.0);
		}
		std::cout << delta << std::endl;
		prev = delta;
	}


	auto transf = Transform::mix(
		_transform[lessOrEqual - _time.begin()],
		_transform[greaterOrEqual - _time.begin()],
		delta
	);

	return transf;
}

Animation::~Animation()
{
}

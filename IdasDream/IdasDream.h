#pragma once

#include "Application.h"

class IdasDream
	: public Application
{
public:
	IdasDream();
	virtual ~IdasDream();

	void init() override;
	void render(float dt) override;
	void destroy() override;
};


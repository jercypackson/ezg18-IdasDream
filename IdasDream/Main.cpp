#include "pch.h"
#include <iostream>
#include "IdasDream.h"
#include "Application.h"
#include <memory>

int main(int argc, char** argv)
{
	std::unique_ptr<IdasDream> app = std::make_unique<IdasDream>();
	if (!app->start()) {
		std::getchar();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
#include "pch.h"
#include <iostream>
#include "IdasDream.h"
#include "Application.h"
#include <memory>
#include <INIReader.h>
#include "Extensions.h"

int main(int argc, char** argv)
{
	// Read settings
	INIReader reader(Extensions::assets + "settings.ini");

	std::unique_ptr<IdasDream> app = std::make_unique<IdasDream>(
		reader.GetInteger("WindowParams", "width", 1600),
		reader.GetInteger("WindowParams", "height", 800),
		reader.GetBoolean("WindowParams", "fullscreen", false)
	);

	if (!app->start()) {
		std::getchar();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
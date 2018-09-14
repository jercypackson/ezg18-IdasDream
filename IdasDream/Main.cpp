#include "pch.h"
#include <iostream>
#include "IdasDream.h"
#include "Application.h"
#include <memory>
#include <INIReader.h>

int main(int argc, char** argv)
{
	// Read settings
	INIReader reader("../assets/settings.ini");
	if (reader.ParseError() < 0) {
		std::cout << "Can't load 'test.ini'\n";
		return EXIT_FAILURE;
	}

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
#include "pch.h"
#include <iostream>
#include "IdasDream.h"
#include "Application.h"
#include <memory>
#include <INIReader.h>
#include "Extensions.h"

int main(int argc, char** argv)
{

	/*
	 *todo: remove
	 */
	 //	std::vector<int> secs = {
	 //		1750,
	 //1810 ,
	 //1820 ,
	 //1825 ,
	 //1845 ,
	 //1880 ,
	 //1880 ,
	 //1915 ,
	 //1925 ,
	 //1935 ,
	 //2005 ,
	 //2015 ,
	 //2025 ,
	 //2155 ,
	 //2165 ,
	 //2175
	 //	};
	 //
	 //	int zero = 5310;
	 //
	 //	std::cout << zero << "," << std::endl;
	 //
	 //	for (int i = 0; i < secs.size() - 1; ++i)
	 //	{
	 //		zero += secs[i + 1] - secs[i];
	 //		std::cout << zero << "," << std::endl;
	 //	}


		  // Read settings
	INIReader reader(Extensions::assets + "settings.ini");

	std::unique_ptr<IdasDream> app = std::make_unique<IdasDream>(
		reader.GetInteger("WindowParams", "width", 1600),
		reader.GetInteger("WindowParams", "height", 900),
		reader.GetBoolean("WindowParams", "fullscreen", false),
		reader.GetInteger("WindowParams", "samples", 1)
		);

	if (!app->start()) {
		std::getchar();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
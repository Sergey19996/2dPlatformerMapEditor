//#include "cGameSettings.h"
#include "Map_Editor.h"

int main()
{
	// Load the settings singleton
	//cGameSettings config;
	//if (!config.LoadConfigFile("assets/config.lua"))
	//{
	//	std::cout << "Failed to load '/assets/config.lua'" << std::endl;
	//	std::cout << "  -> Using default configuration" << std::endl;
	//}



	Map_Editor game;
	if (game.Construct(1024, 768, 1, 1,true))
		game.Start();
	return 0;
}
#include <iostream>
#include "Engine/Engine.h"

int main()
{
	Engine engine;
	//engine.loadModels("Models");

	bool exitLoop = false;
	while (!exitLoop)
	{
		engine.preRender();
		//draw everything else here
		engine.postRender();
		//maybe some game specific shit idk
		exitLoop = engine.checkShouldClose();
	}

	return 0;
}


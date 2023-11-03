#include "Engine.h"
#include <filesystem>
#include <iostream>
#include <regex>


Engine::Engine()
{
	renderer = std::make_unique<Renderer::RenderEngine>(1280, 720);
}

Engine::~Engine()
{
}

void Engine::loadModels(std::string modelsDir)
{
	std::string path = "Models";
	try
	{
		std::filesystem::recursive_directory_iterator fileDirs(path);
		for (const auto& entry : fileDirs)
		{
			std::string entryPath = entry.path().string();
			size_t fbxPos = entryPath.size() - entryPath.rfind(".fbx");
			if (fbxPos == 4)
			{
				while (entryPath.find("\\") != std::string::npos)
				{
					entryPath.replace(entryPath.find("\\"), sizeof("\\") - 1, "/");
				}
				std::cout << "Loading model: " << entryPath << std::endl;
				this->models.push_back(std::make_shared<Renderer::Model>(entryPath));

			}
		}
	}
	catch (std::filesystem::filesystem_error error)
	{
		std::cout << "Problem loading model files: " << error.what() << "\n";
	}
	
	//loadMaterials();
		
}

void Engine::loadMaterials(std::string materialsDir)
{

}

void Engine::preRender()
{
	renderer->preDrawEvent();
}

void Engine::postRender()
{
	renderer->postDrawEvent();
}

bool Engine::checkShouldClose()
{
	return renderer->checkWindowClose();
}


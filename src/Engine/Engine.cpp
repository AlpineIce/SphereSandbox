#include "Engine.h"
#include <filesystem>
#include <iostream>
#include <regex>


Engine::Engine()
{
	renderer = std::make_unique<Renderer::RenderEngine>(1600, 900);
	physicsEngine = std::make_unique<Physics::PhysicsEngine>(&physicsLock);
	time = renderer->getTimePointer();
}

Engine::~Engine()
{
}

//---------LOAD FUNCTIONS----------//

void Engine::loadShaders(std::string shadersDir)
{
	try
	{
		std::filesystem::directory_iterator fileDirs(shadersDir);
		for (const auto& entry : fileDirs)
		{
			std::string entryPath = entry.path().string();
			if (entryPath.rfind("PBR") != std::string::npos)
			{
				std::cout << "Loading shader PBR" << entryPath << std::endl;

				while (entryPath.find("\\") != std::string::npos)
				{
					entryPath.replace(entryPath.find("\\"), sizeof("\\") - 1, "/");
				}

				//make shaders from the directory set
				std::filesystem::directory_iterator PBRfiles(entryPath);
				std::vector<std::string> shaderFiles(2);
				for (const auto& entry2 : PBRfiles)
				{
					std::string PBRfile = entry2.path().string();
					while (PBRfile.find("\\") != std::string::npos)
					{
						PBRfile.replace(PBRfile.find("\\"), sizeof("\\") - 1, "/");
					}
					if (PBRfile.find("vertex.glsl") != std::string::npos)
					{
						shaderFiles[0] = PBRfile;
					}
					else if (PBRfile.find("fragment.glsl") != std::string::npos)
					{
						shaderFiles[1] = PBRfile;
					}
					
				}
				if (shaderFiles[0].find("vertex.glsl") != std::string::npos && shaderFiles[1].find("fragment.glsl") != std::string::npos)
				{
					this->shaders[ShaderType::PBR] = std::make_shared<Renderer::Shader>(shaderFiles[0].c_str(), shaderFiles[1].c_str(), renderer->getCamera());
				}
				else
				{
					throw std::runtime_error("Shader loading of " + shaderFiles[0] + " " + shaderFiles[1] + " failed.");
				}
			}
		}

	}
	catch (std::filesystem::filesystem_error error)
	{
		throw std::runtime_error("Problem loading model files");
	}
}

void Engine::loadModels(std::string modelsDir)
{
	try
	{
		std::filesystem::recursive_directory_iterator fileDirs(modelsDir);
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

				size_t offset = entryPath.rfind("/") + 1;
				std::string modelName = entryPath.substr(offset, entryPath.rfind(".fbx") - offset);
				this->models[modelName] = std::make_shared<Renderer::Model>(entryPath);

			}
		}
	}
	catch (std::filesystem::filesystem_error error)
	{
		std::cout << "Problem loading model files: " << error.what() << "\n";
	}
}

void Engine::loadMaterials(std::string materialsDir)
{
	try
	{
		std::filesystem::recursive_directory_iterator fileDirs(materialsDir);
		for (const auto& entry : fileDirs)
		{
			std::string entryPath = entry.path().string();
			if (entryPath.rfind("m_") != std::string::npos && entryPath.rfind(".") == std::string::npos) //crudely dangerous way of check if is a m_folder
			{
				std::cout << "Loading material: " << entryPath << std::endl;
				while (entryPath.find("\\") != std::string::npos)
				{
					entryPath.replace(entryPath.find("\\"), sizeof("\\") - 1, "/");
				}

				size_t offset = entryPath.rfind(materialsDir) + materialsDir.size() + 1;
				std::string matName = entryPath.substr(offset);
				matName = matName.replace(matName.find("/"), sizeof("/") - 1, "_");
				this->materials[matName] = std::make_shared<Renderer::Material>(ShaderType::PBR, entryPath, shaders[ShaderType::PBR].get());
			}
		}
	}
	catch (std::filesystem::filesystem_error error)
	{
		std::cout << "Problem loading model files: " << error.what() << "\n";
	}
}

//---------OBJECT REFERENCE FUNCTIONS----------//

unsigned long Engine::addModelInstPtr(Renderer::ModelInstance* inst)
{
	modelInstPtrs[(unsigned long)modelInstPtrs.size()] = inst;
	return (unsigned long)modelInstPtrs.size() - 1;
}

void Engine::removeModelInstPtr(unsigned long location)
{
	modelInstPtrs.erase(location);
}

void Engine::changeModelInstPtr(Renderer::ModelInstance* inst, unsigned long location)
{
	modelInstPtrs.at(location) = inst;
}

unsigned long Engine::addCollisionPtr(Physics::ColliderType type, Physics::PhysicsObject* object)
{
	switch (type)
	{
	case Physics::ColliderType::DYNAMIC:
		dynamicCollisionPtrs[(unsigned long)dynamicCollisionPtrs.size()] = object;
		return (unsigned long)dynamicCollisionPtrs.size() - 1;

	}

	return 0;
}

void Engine::removeCollisionPtr(Physics::ColliderType type, unsigned long location)
{
	if (type == Physics::ColliderType::DYNAMIC)
	{
		dynamicCollisionPtrs.erase(location);
	}
}

void Engine::changeCollisionPtr(Physics::ColliderType type, Physics::PhysicsObject* object, unsigned long location)
{
	if (type == Physics::ColliderType::DYNAMIC)
	{
		dynamicCollisionPtrs.at(location) = object;
	}
}

//---------RENDER EVENTS----------//


void Engine::preRender()
{
	renderer->preDrawEvent();
}

void Engine::renderEvent()
{
	//render all modelInst with material type of PBR		TODO MAKE IT WHERE THIS ACTUALLY CHECKS THE MATERIAL TYPE
	getShaderByType(ShaderType::PBR)->bind();

	//start by updating the lights and camera
	getShaderByType(ShaderType::PBR)->setCameraPosition(getRenderer()->getCamera()->getPosition());
	getShaderByType(ShaderType::PBR)->updateLights(getPointLights(), getDirectLight(), getAmbientLight());

	//maybe do some shadow mapping i havent implemented yet?

	//render the model
	for (auto  const [key, val] : modelInstPtrs)
	{
		if (val != NULL)
		{
			val->render(*renderer->getCamera());
		}
	}
	getShaderByType(ShaderType::PBR)->unbind();
}

void Engine::postRender()
{
	renderer->postDrawEvent();
}

bool Engine::checkShouldClose()
{
	return renderer->checkWindowClose();
}

//---------PRE/POST LOOP EVENTS----------//

void Engine::preLoop()
{
	physicsThread = std::thread(&Physics::PhysicsEngine::initLoop, physicsEngine.get(), &dynamicCollisionPtrs);
}

void Engine::postLoop()
{
	physicsEngine->stopLoop();
	physicsThread.join();
	
}


#include "Engine.h"
#include <filesystem>
#include <iostream>
#include <regex>


Engine::Engine()
{
	renderer = std::make_unique<Renderer::RenderEngine>(1280, 720);
	time = renderer->getTimePointer();
}

Engine::~Engine()
{
}

void Engine::loadShaders(std::string shadersDir)
{
	std::string path = shadersDir;
	try
	{
		std::filesystem::directory_iterator fileDirs(path);
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
		std::cout << "Problem loading model files: " << error.what() << "\n";
	}
}

void Engine::loadModels(std::string modelsDir)
{
	std::string path = modelsDir;
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
	std::string path = materialsDir;
	try
	{
		std::filesystem::recursive_directory_iterator fileDirs(path);
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






unsigned long Engine::addModelInstPtr(Renderer::ModelInstance* inst)
{
	modelInstPtrs.push_back(inst); return modelInstPtrs.size() - 1;
}

void Engine::removeModelInstPtr(unsigned long location)
{
	//this is a rather inneficient process, but it helps that quickly appearing
	//objects will also be quickly deleted. Will change if it ever becomes a bottleneck
	modelInstPtrs.erase(modelInstPtrs.begin() + location);
}

void Engine::changeModelInstPtr(Renderer::ModelInstance* inst, unsigned long location)
{
	modelInstPtrs.at(location) = inst;
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

	//render the models
	for (const Renderer::ModelInstance* inst : modelInstPtrs)
	{
		if (inst != NULL)
		{
			inst->render(renderer->getCamera());
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


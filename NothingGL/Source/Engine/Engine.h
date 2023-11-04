#pragma once
#include "Render/Renderer.h"
#include "Render/Model.h"
#include <map>
#include <vector>

class Engine
{
private:
	int width;
	int height;

	std::vector<std::shared_ptr<Renderer::Material>> materials;
	std::vector<std::shared_ptr<Renderer::Model>> models;
	std::unique_ptr<Renderer::RenderEngine> renderer;

public:
	Engine();
	~Engine();

	void loadModels(std::string modelsDir);
	void loadMaterials(std::string materialsDir);

	void preRender();
	void postRender();
	bool checkShouldClose();

};
#pragma once
#include "Render/Renderer.h"
#include "Render/Model.h"
#include "Render/Material.h"
#include <map>
#include <vector>

class Engine
{
private:
	int width;
	int height;

	std::map<MaterialType, std::shared_ptr<Renderer::Shader>> shaders;
	std::map<std::string, std::shared_ptr<Renderer::Material>> materials;
	std::map<std::string, std::shared_ptr<Renderer::Model>> models;
	
	std::unique_ptr<Renderer::RenderEngine> renderer;

public:
	Engine();
	~Engine();

	const double* time;

	void loadShaders(std::string shadersDir);
	void loadModels(std::string modelsDir);
	void loadMaterials(std::string materialsDir);

	inline std::map<MaterialType, std::shared_ptr<Renderer::Shader>>* getShaders() { return &shaders; }
	inline std::map<std::string, std::shared_ptr<Renderer::Material>>* getMaterials() { return &materials; }
	inline std::map<std::string, std::shared_ptr<Renderer::Model>>* getModels() { return &models; }
	inline Renderer::Material* getMaterialFromName(std::string name) { return materials[name].get(); }
	inline Renderer::RenderEngine* getRenderer() const { return renderer.get(); }

	void preRender();
	void postRender();
	bool checkShouldClose();

};
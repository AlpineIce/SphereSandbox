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

	//shaders, materials, and models; all loaded on startup
	std::map<MaterialType, std::shared_ptr<Renderer::Shader>> shaders;
	std::map<std::string, std::shared_ptr<Renderer::Material>> materials;
	std::map<std::string, std::shared_ptr<Renderer::Model>> models;

	//point and directional lights
	std::vector<std::shared_ptr<PointLight>> pointLights;
	std::shared_ptr<DirectionalLight> directLight; //this could likely be used more than once in a vector
	
	std::unique_ptr<Renderer::RenderEngine> renderer;

public:
	Engine();
	~Engine();

	const double* time;

	void loadShaders(std::string shadersDir);
	void loadModels(std::string modelsDir);
	void loadMaterials(std::string materialsDir);

	inline std::map<MaterialType, std::shared_ptr<Renderer::Shader>>*	getShaders() { return &shaders; }
	inline Renderer::Shader*											getShaderByType(MaterialType type) { return shaders[type].get(); }
	inline std::map<std::string, std::shared_ptr<Renderer::Material>>*	getMaterials() { return &materials; }
	inline std::map<std::string, std::shared_ptr<Renderer::Model>>*		getModels() { return &models; }
	inline Renderer::Model*												getModelFromName(std::string name) { if (models.count(name)) return models[name].get(); }
	inline Renderer::Material*											getMaterialFromName(std::string name) { if (materials.count(name)) return materials[name].get(); }
	inline std::map<unsigned int, Renderer::Material*>					getModelMaterialSlots(std::string name) { if(models.count(name)) return models[name]->getMaterialSlots(); }
	inline std::vector<std::shared_ptr<PointLight>>*					getPointLights() { return &pointLights; }
	inline std::shared_ptr<DirectionalLight>							getDirectLight() { return directLight; }

	inline Renderer::RenderEngine*										getRenderer() const { return renderer.get(); }

	inline void setModelMaterialSlots(std::string name, std::map<unsigned int, Renderer::Material*> matSlots) { models[name]->setMaterialSlots(matSlots); }
	inline void createDirectionalLight() { directLight = std::make_shared<DirectionalLight>(); }

	void preRender();
	void postRender();
	bool checkShouldClose();

};
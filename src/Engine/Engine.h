#pragma once
#include "Render/Renderer.h"
#include "Render/Model.h"
#include "Render/Material.h"
#include "Physics/Physics.h"

#include <map>
#include <iostream>
#include <vector>
#include <thread>

typedef std::map<unsigned int, Renderer::Material*>* MaterialSlots;

class Engine
{
private:
	int width;
	int height;

	//shaders, materials, and models; all loaded on startup
	std::map<ShaderType, std::shared_ptr<Renderer::Shader>> shaders;
	std::map<std::string, std::shared_ptr<Renderer::Material>> materials;
	std::map<std::string, std::shared_ptr<Renderer::Model>> models;

	//point and directional lights
	std::map<unsigned int, Light::PointLight*> pointLights;
	Light::DirectionalLight* directLight;
	Light::AmbientLight* ambientLight;

	//actor component references
	std::map<unsigned long, Renderer::ModelInstance*> modelInstPtrs;

	std::map<unsigned long, Physics::PhysicsObject*> dynamicCollisionPtrs;
	std::map<unsigned long, Physics::PhysicsObject*> staticCollisionPtrs;
	std::map<unsigned long, Physics::PhysicsObject*> overlapCollisionPtrs;
	
	//render, physics, etc
	Renderer::RenderEngine renderer; //renderer "object"
	Physics::PhysicsEngine physicsEngine;

	//threads
	std::thread physicsThread;
	std::mutex physicsLock;

public:
	Engine();
	~Engine();

	const double* time;

	void loadShaders(std::string shadersDir);
	void loadModels(std::string modelsDir);
	void loadMaterials(std::string materialsDir);

	//TODO probably define all these functions within the cpp file? honestly not sure 
	//yet if its a good idea to keep doing these same line function definitions

	//shader and material getter functions
	inline std::map<ShaderType, std::shared_ptr<Renderer::Shader>>*		getShaders() { return &shaders; }
	inline Renderer::Shader*											getShaderByType(ShaderType type) { return shaders.count(type) ? shaders[type].get() : NULL; }
	inline std::map<std::string, std::shared_ptr<Renderer::Material>>*	getMaterials() { return &materials; }
	inline Renderer::Material*											getMaterialFromName(std::string name) { return materials.count(name) ? materials[name].get() : NULL; }
	//model getter functions
	inline std::map<std::string, std::shared_ptr<Renderer::Model>>*		getModels() { return &models; }
	inline Renderer::Model*												getModelFromName(std::string name) { return models.count(name) ? models[name].get() : NULL; }
	inline std::map<unsigned int, Renderer::Material*>*					getModelMaterialSlots(std::string name) { return models.count(name) ? models[name]->getMaterialSlots() : NULL; }
	//engines
	inline Renderer::RenderEngine*										getRenderer() { return &renderer; }
	inline Physics::PhysicsEngine*										getPhysicsEngine() { return &physicsEngine; }

	//light creation functions
	inline void setDirectionalLight(Light::DirectionalLight* light)		{ directLight = light; }
	inline void setAmbientLight(Light::AmbientLight* light)			{ ambientLight = light; }

	//functions for pushing back pointers for actor components, as well as returning their position in the vector
	unsigned long addModelInstPtr(Renderer::ModelInstance* inst);
	void removeModelInstPtr(unsigned long location);
	void changeModelInstPtr(Renderer::ModelInstance* inst, unsigned long location);

	//same as above but for collision
	unsigned long addCollisionPtr(Physics::ColliderType type, Physics::PhysicsObject* object);
	void removeCollisionPtr(Physics::ColliderType type, unsigned long location);
	void changeCollisionPtr(Physics::ColliderType type, Physics::PhysicsObject* object, unsigned long location);

	//adding point lights
	unsigned int addPointLightPtr(Light::PointLight* light);
	void removePointLightPtr(unsigned int location);
	void changePointLightPtr(Light::PointLight* light, unsigned int location);
	
	//events
	void preRender();
	void renderEvent();
	void postRender();
	bool checkShouldClose();

	void preLoop();
	void postLoop();

};
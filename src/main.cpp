#include <iostream>
#include <math.h>
#include <thread>
#include <vector>
#include <memory>

#include "Engine/Actors/PhysicalActor.h"
#include "Engine/Engine.h"

int main()
{
	Engine engine;
	engine.loadShaders("Shaders");
	engine.loadModels("Models");
	engine.loadMaterials("Materials");

	//Material setup. This is kind of unsafe, but the number of material slots
	//can be grabbed by doing cup_m.size(), at least. Ill probably fix it in the future
	MaterialSlots sphere_m = engine.getModelMaterialSlots("Sphere");
	MaterialSlots cube_m = engine.getModelMaterialSlots("Cube");
	if (sphere_m)
	{
		sphere_m->at(0) = engine.getMaterialFromName("Sphere_m_Sphere");
		cube_m->at(0) = engine.getMaterialFromName("Sphere_m_Sphere");
	}

	std::vector<std::unique_ptr<Actor::PhysicalActor>> spheres(250);
	for (int i = 0; i < spheres.size(); i++)
	{
		spheres.at(i) = std::make_unique<Actor::PhysicalActor>(engine.getModelFromName("Sphere"), &engine, Physics::ColliderType::DYNAMIC, Physics::PhysicsShape::SPHERE);
		Actor::Transformation transform;
		transform.location = glm::vec3(sin((float)i / 4.0f) * 50.0f, cos((float)i / 4.0f) * 50.0f, sin((float)i) * 10.0f);
		spheres.at(i)->transform(transform);

	}

	//create some lights //TODO TURN THESE INTO ACTORS ALSO
	engine.createDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f));
	engine.getPointLights()->push_back(std::make_shared<PointLight>(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(1.0f, 1.0f, 0.0f)));
	engine.getPointLights()->push_back(std::make_shared<PointLight>(glm::vec3(-10.0f, 10.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	engine.getPointLights()->push_back(std::make_shared<PointLight>(glm::vec3(10.0f, 10.0f, -10.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	engine.getPointLights()->push_back(std::make_shared<PointLight>(glm::vec3(-10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	engine.getDirectLight()->setRotation(0.0f, 45.0f);
	
	engine.createAmbientLight();
	engine.getAmbientLight()->setPower(1.5f);

	
	engine.preLoop(); //physics engine runs seperate from renderer loop below, has its own thread joining here

	bool exitLoop = false;
	unsigned long frameNum = 0;
	while (!exitLoop)
	{
		for (int i = 0; i < spheres.size(); i++)
		{
			spheres.at(i)->updatePhysics();
		}

		engine.preRender();
		engine.renderEvent();
		engine.postRender();
		
		exitLoop = engine.checkShouldClose();
	}
	engine.postLoop();

	return 0;
}


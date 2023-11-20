#include <iostream>
#include <math.h>
#include <thread>

#include "Actors/PhysicalActor.h"
#include "Engine/Engine.h"

int main()
{
	//TODO load all 3 in different threads, syncing when loadedf
	Engine engine;
	engine.loadShaders("Shaders");
	engine.loadModels("Models");
	engine.loadMaterials("Materials");

	//Material setup. This is kind of unsafe, but the number of material slots
	//can be grabbed by doing cup_m.size(), at least. Ill probably fix it in the future
	MaterialSlots cup_m = engine.getModelMaterialSlots("SkullCup");
	if (cup_m)
	{
		cup_m->at(0) = engine.getMaterialFromName("SkulCup_m_Teeth");
		cup_m->at(1) = engine.getMaterialFromName("SkulCup_m_Base");
		cup_m->at(2) = engine.getMaterialFromName("SkulCup_m_Bone");
		cup_m->at(3) = engine.getMaterialFromName("SkulCup_m_Gem");
	}

	MaterialSlots sphere_m = engine.getModelMaterialSlots("Icosphere");
	MaterialSlots cube_m = engine.getModelMaterialSlots("Cube");
	if (sphere_m)
	{
		sphere_m->at(0) = engine.getMaterialFromName("Sphere_m_Sphere");
		cube_m->at(0) = engine.getMaterialFromName("Sphere_m_Sphere");
	}

	MaterialSlots landscape_m = engine.getModelMaterialSlots("Landscape");
	if (sphere_m)
	{
		landscape_m->at(0) = engine.getMaterialFromName("Landscape_m_Grass");
	}

	//creation of actors
	std::unique_ptr<Actor::PhysicalActor> icosphere = 
		std::make_unique<Actor::PhysicalActor>(engine.getModelFromName("Icosphere"), &engine, Physics::ColliderType::DYNAMIC, Physics::PhysicsShape::SPHERE);
	std::unique_ptr<Actor::PhysicalActor> testModel =
		std::make_unique<Actor::PhysicalActor>(engine.getModelFromName("SkullCup"), &engine, Physics::ColliderType::STATIC, Physics::PhysicsShape::SPHERE);
	std::unique_ptr<Actor::PhysicalActor> testModel2 =
		std::make_unique<Actor::PhysicalActor>(engine.getModelFromName("SkullCup"), &engine, Physics::ColliderType::STATIC, Physics::PhysicsShape::SPHERE);
	std::unique_ptr<Actor::PhysicalActor> landscape =
		std::make_unique<Actor::PhysicalActor>(engine.getModelFromName("Icosphere"), &engine, Physics::ColliderType::DYNAMIC, Physics::PhysicsShape::SPHERE);
	std::unique_ptr<Actor::PhysicalActor> bruhcube =
		std::make_unique<Actor::PhysicalActor>(engine.getModelFromName("Icosphere"), &engine, Physics::ColliderType::DYNAMIC, Physics::PhysicsShape::SPHERE);	


	Actor::Transformation cubeTransform;
	cubeTransform.location = { 2.0f, 2.5f, 0.0f };
	bruhcube->transform(cubeTransform);

	Actor::Transformation sphereTransform;
	sphereTransform.location = { -2.0f, 2.5f, 0.0f };
	icosphere->transform(sphereTransform);

	//create some lights //TODO TURN THESE INTO ACTORS ALSO
	engine.createDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f));
	//engine.getPointLights()->push_back(std::make_shared<PointLight>(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(1.0f, 1.0f, 0.0f)));
	//engine.getPointLights()->push_back(std::make_shared<PointLight>(glm::vec3(-10.0f, 10.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	//engine.getPointLights()->push_back(std::make_shared<PointLight>(glm::vec3(10.0f, 10.0f, -10.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	//engine.getPointLights()->push_back(std::make_shared<PointLight>(glm::vec3(-10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	engine.getDirectLight()->setRotation(0.0f, 45.0f);
	
	engine.createAmbientLight();
	engine.getAmbientLight()->setPower(1.5f);

	
	engine.preLoop(); //physics engine runs seperate from renderer loop below, has its own thread joining here

	bool exitLoop = false;
	unsigned long frameNum = 0;
	while (!exitLoop)
	{
		//update actors physics
		icosphere->transformPhysics();
		//testModel->transformPhysics();
		//testModel2->transformPhysics();
		landscape->transformPhysics();
		bruhcube->transformPhysics();

		//debug if statement 
		if (frameNum == 1000)
		{
			//bruhcube.reset();
		}
		frameNum++;

		Actor::Transformation modelTransform;
		modelTransform.location = glm::vec3(sin(*engine.time) * 3.0f, 4.0f, cos(*engine.time) * 3.0f);
		modelTransform.rotation = glm::quat(sin(*engine.time), 0.0f, cos(*engine.time), 0.0f);
		testModel->transform(modelTransform);

		Actor::Transformation model2Transform;
		model2Transform.location = glm::vec3(-sin(*engine.time) * 3.0f, 4.0f, -cos(*engine.time) * 3.0f);
		model2Transform.rotation = glm::quat(sin(*engine.time * 0.2f), 0.0f, cos(*engine.time * 0.2f), 0.0f);
		testModel2->transform(model2Transform);

		engine.preRender();
		engine.renderEvent();
		engine.postRender();
		
		exitLoop = engine.checkShouldClose();
	}
	engine.postLoop();

	return 0;
}


#include <iostream>
#include "Engine/Engine.h"
#include <math.h>

int main()
{
	Engine engine;
	engine.loadShaders("Shaders");
	engine.loadModels("Models");
	engine.loadMaterials("Materials");

	//skull cup materials
	MaterialSlots cup_m = engine.getModelMaterialSlots("SkullCup");
	if (cup_m) //TODO TURN THIS INTO A FUNCTION WHICH TAKES IN A LIST OF STRINGS AS MATERIAL NAMES
	{
		//This is arguably unsafe, but the number of material slots can be grabbed by doing cup_m.size(), at least. Ill probably fix it in the future
		cup_m->at(0) = engine.getMaterialFromName("SkulCup_m_Teeth");
		cup_m->at(1) = engine.getMaterialFromName("SkulCup_m_Base");
		cup_m->at(2) = engine.getMaterialFromName("SkulCup_m_Bone");
		cup_m->at(3) = engine.getMaterialFromName("SkulCup_m_Gem");
	}

	//sphere and ground material
	MaterialSlots sphere_m = engine.getModelMaterialSlots("Icosphere");
	MaterialSlots ground_m = engine.getModelMaterialSlots("ground");
	if (sphere_m)
	{
		sphere_m->at(0) = engine.getMaterialFromName("Sphere_m_Sphere");
		ground_m->at(0) = engine.getMaterialFromName("Sphere_m_Sphere");
	}
	
	Renderer::ModelInstance icosphere(engine.getModelFromName("Icosphere"));
	Renderer::ModelInstance testModel(engine.getModelFromName("SkullCup"));
	Renderer::ModelInstance testModel2(engine.getModelFromName("SkullCup"));
	Renderer::ModelInstance ground1(engine.getModelFromName("ground"));
	Renderer::ModelInstance bruhcube(engine.getModelFromName("Cube"));

	Renderer::Transformation cubeTransform;
	cubeTransform.location = { 5.0f, 0.0f, 0.0f };
	bruhcube.setTransformation(cubeTransform);

	Renderer::Transformation backpackTransform;
	backpackTransform.location = { -5.0f, 0.0f, 0.0f };
	icosphere.setTransformation(backpackTransform);

	Renderer::Transformation groundTransform;
	groundTransform.rotation = glm::quat(0.707106f, -0.707106f, 0.0f, 0.0f);
	groundTransform.location = (glm::vec3(0.0f, -4.0f, 0.0f));
	ground1.setTransformation(groundTransform);

	//create some lights
	engine.createDirectionalLight();
	engine.getPointLights()->push_back(std::make_shared<PointLight>(glm::vec3(5.0f, -3.5f, 0.0f)));
	engine.getPointLights()->push_back(std::make_shared<PointLight>(glm::vec3(-2.0f, -3.0f, -4.0f)));

	engine.getDirectLight()->setRotation(0.0f, 30.0f);

	bool exitLoop = false;
	while (!exitLoop)
	{
		engine.preRender();
		
		//TODO possibly launch these 4 lines in a thread and do other transformations on main thread?
		engine.getShaderByType(MaterialType::PBR)->bind();
		engine.getShaderByType(MaterialType::PBR)->setCameraPosition(engine.getRenderer()->getCamera()->getPosition());
		engine.getShaderByType(MaterialType::PBR)->updateLights(*engine.getPointLights(), engine.getDirectLight());
		engine.getShaderByType(MaterialType::PBR)->unbind();
		

		Renderer::Transformation modelTransform;
		modelTransform.location = glm::vec3(sin(*engine.time) * 2.0f, cos(*engine.time) * 2.0f, 0.0f);
		modelTransform.rotation = glm::quat(sin(*engine.time), 0.0f, cos(*engine.time), 0.0f);

		icosphere.render(engine.getRenderer()->getCamera());
		ground1.render(engine.getRenderer()->getCamera());
		testModel.setTransformation(modelTransform);
		testModel.render(engine.getRenderer()->getCamera());
		testModel2.render(engine.getRenderer()->getCamera());
		bruhcube.render(engine.getRenderer()->getCamera());


		engine.postRender();
		//maybe some game specific shit idk
		exitLoop = engine.checkShouldClose();
	}

	return 0;
}


#include <iostream>
#include "Engine/Engine.h"
#include <math.h>

int main()
{
	Engine engine;
	engine.loadShaders("Shaders");
	engine.loadModels("Models");
	engine.loadMaterials("Materials");

	
	//some shader variables
	

	//skull cup materials
	auto cup_m = engine.getModelMaterialSlots("SkullCup");
	if (cup_m.size())
	{
		cup_m[0] = engine.getMaterialFromName("SkulCup_m_Teeth");
		cup_m[1] = engine.getMaterialFromName("SkulCup_m_Base");
		cup_m[2] = engine.getMaterialFromName("SkulCup_m_Bone");
		cup_m[3] = engine.getMaterialFromName("SkulCup_m_Gem");
		engine.setModelMaterialSlots("SkullCup", cup_m);
		engine.setModelMaterialSlots("ground", cup_m);
		engine.setModelMaterialSlots("Cube", cup_m);
	}

	//backpack material
	auto backpack_m = engine.getModelMaterialSlots("Icosphere");
	if (backpack_m.size())
	{
		backpack_m[0] = engine.getMaterialFromName("Backpack_m_Base");
		engine.setModelMaterialSlots("Icosphere", backpack_m);
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


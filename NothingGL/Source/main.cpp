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
	auto backpack_m = engine.getModelMaterialSlots("Backpack");
	if (backpack_m.size())
	{
		backpack_m[0] = engine.getMaterialFromName("Backpack_m_Base");
		engine.setModelMaterialSlots("Backpack", backpack_m);
	}
	
	Renderer::ModelInstance backpack(engine.getModelFromName("Backpack"));
	Renderer::ModelInstance testModel(engine.getModelFromName("SkullCup"));
	Renderer::ModelInstance testModel2(engine.getModelFromName("SkullCup"));
	Renderer::ModelInstance ground1(engine.getModelFromName("ground"));
	Renderer::ModelInstance bruhcube(engine.getModelFromName("Cube"));

	Renderer::Transformation cubeTransform;
	cubeTransform.location = { 5.0f, 0.0f, 0.0f };
	bruhcube.setTransformation(cubeTransform);

	Renderer::Transformation backpackTransform;
	backpackTransform.location = { -5.0f, 0.0f, 0.0f };
	backpack.setTransformation(backpackTransform);

	Renderer::Transformation groundTransform;
	groundTransform.rotation = glm::quat(0.707106f, -0.707106f, 0.0f, 0.0f);
	groundTransform.location = (glm::vec3(0.0f, -4.0f, 0.0f));
	ground1.setTransformation(groundTransform);

	bool exitLoop = false;
	while (!exitLoop)
	{
		engine.preRender();
		

		Renderer::Transformation lightTransform;
		lightTransform.location = { sin(*engine.time) * 5.0f, 0.0f, cos(*engine.time) * 5.0f };
		
		engine.getShaderByType(MaterialType::PBR)->bind();
		engine.getShaderByType(MaterialType::PBR)->setFloat3("lightPos", lightTransform.location);
		engine.getShaderByType(MaterialType::PBR)->setFloat3("viewPos", engine.getRenderer()->getCamera()->getPosition());
		engine.getShaderByType(MaterialType::PBR)->unbind();

		Renderer::Transformation modelTransform;
		modelTransform.location = glm::vec3(sin(*engine.time) * 2.0f, cos(*engine.time) * 2.0f, 0.0f);
		modelTransform.rotation = glm::quat(sin(*engine.time), 0.0f, cos(*engine.time), 0.0f);

		backpack.render(engine.getRenderer()->getCamera());
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


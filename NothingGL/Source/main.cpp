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

	//sphere and cube material
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
	
	Renderer::ModelInstance icosphere(engine.getModelFromName("Icosphere"));
	Renderer::ModelInstance testModel(engine.getModelFromName("SkullCup"));
	Renderer::ModelInstance testModel2(engine.getModelFromName("SkullCup"));
	Renderer::ModelInstance landscape(engine.getModelFromName("Landscape"));
	Renderer::ModelInstance bruhcube(engine.getModelFromName("Cube"));

	Renderer::Transformation cubeTransform;
	cubeTransform.location = { 5.0f, 3.0f, 0.0f };
	bruhcube.setTransformation(cubeTransform);

	Renderer::Transformation sphereTransform;
	sphereTransform.location = { -5.0f, 3.0f, 0.0f };
	icosphere.setTransformation(sphereTransform);

	//create some lights
	engine.createDirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f));
	//engine.getPointLights()->push_back(std::make_shared<PointLight>(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(1.0f, 1.0f, 0.0f)));
	//engine.getPointLights()->push_back(std::make_shared<PointLight>(glm::vec3(-10.0f, 10.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	//engine.getPointLights()->push_back(std::make_shared<PointLight>(glm::vec3(10.0f, 10.0f, -10.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	//engine.getPointLights()->push_back(std::make_shared<PointLight>(glm::vec3(-10.0f, 10.0f, 10.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	engine.getDirectLight()->setRotation(0.0f, 45.0f);
	
	engine.createAmbientLight();
	engine.getAmbientLight()->setPower(1.5f);

	bool exitLoop = false;
	while (!exitLoop)
	{
		engine.preRender();
		
		//TODO possibly launch these 4 lines in a thread and do other transformations on main thread?
		engine.getShaderByType(MaterialType::PBR)->bind();
		engine.getShaderByType(MaterialType::PBR)->setCameraPosition(engine.getRenderer()->getCamera()->getPosition());
		engine.getShaderByType(MaterialType::PBR)->updateLights(engine.getPointLights(), engine.getDirectLight(), engine.getAmbientLight());
		//engine.getShaderByType(MaterialType::PBR)->unbind();
		

		Renderer::Transformation modelTransform;
		modelTransform.location = glm::vec3(sin(*engine.time) * 2.0f, 5.0f, cos(*engine.time) * 2.0f);
		modelTransform.rotation = glm::quat(sin(*engine.time), 0.0f, cos(*engine.time), 0.0f);
		testModel.setTransformation(modelTransform);

		Renderer::Transformation model2Transform;
		model2Transform.location = glm::vec3(-sin(*engine.time) * 2.0f, 5.0f, -cos(*engine.time) * 2.0f);
		model2Transform.rotation = glm::quat(sin(*engine.time * 0.2f), 0.0f, cos(*engine.time * 0.2f), 0.0f);
		testModel2.setTransformation(model2Transform);

		icosphere.render(engine.getRenderer()->getCamera());
		landscape.render(engine.getRenderer()->getCamera());
		
		testModel.render(engine.getRenderer()->getCamera());
		testModel2.render(engine.getRenderer()->getCamera());
		bruhcube.render(engine.getRenderer()->getCamera());

		engine.getShaderByType(MaterialType::PBR)->unbind();
		engine.postRender();
		//maybe some game specifics idk
		exitLoop = engine.checkShouldClose();
	}

	return 0;
}


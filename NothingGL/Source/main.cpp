#include <iostream>
#include "Engine/Engine.h"
#include <math.h>

int main()
{
	Engine engine;
	engine.loadModels("Models");
	engine.loadShaders("Shaders");
	engine.loadMaterials("Materials");

	//skull cup material initialization
	auto mat = engine.getModels()->at("SkullCup")->getMaterialSlots();
	mat[0] = engine.getMaterialFromName("SkulCup_m_Teeth");
	mat[1] = engine.getMaterialFromName("SkulCup_m_Base");
	mat[2] = engine.getMaterialFromName("SkulCup_m_Bone");
	mat[3] = engine.getMaterialFromName("SkulCup_m_Gem");
	engine.getModels()->at("SkullCup")->setMaterialSlots(mat);
	engine.getModels()->at("Cube")->setMaterialSlots(mat);
	engine.getModels()->at("ground")->setMaterialSlots(mat);

	Renderer::ModelInstance testModel(engine.getModels()->at("SkullCup").get());
	Renderer::ModelInstance testModel2(engine.getModels()->at("SkullCup").get());
	Renderer::ModelInstance ground1(engine.getModels()->at("ground").get());
	Renderer::ModelInstance bruhcube(engine.getModels()->at("Cube").get());

	Renderer::Transformation cubeTransform;
	cubeTransform.location = { 5.0f, 0.0f, 0.0f };
	bruhcube.setTransformation(cubeTransform);


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

		engine.getShaders()->at(MaterialType::PBR)->bind();
		engine.getShaders()->at(MaterialType::PBR)->setFloat3("lightPos", lightTransform.location);
		engine.getShaders()->at(MaterialType::PBR)->setFloat3("viewPos", engine.getRenderer()->getCamera()->getPosition());
		engine.getShaders()->at(MaterialType::PBR)->unbind();

		Renderer::Transformation modelTransform;
		modelTransform.location = glm::vec3(sin(*engine.time) * 2.0f, cos(*engine.time) * 2.0f, 0.0f);
		modelTransform.rotation = glm::quat(sin(*engine.time), 0.0f, cos(*engine.time), 0.0f);


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


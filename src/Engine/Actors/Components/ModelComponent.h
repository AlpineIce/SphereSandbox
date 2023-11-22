#pragma once
#include "Render/Model.h"
#include "Engine.h"

//MODEL COMPONENT
//This class is pretty much the actor component for rendering anything on screen.
//A current limitation of it is that materials are setup for the parent model,
//but there is a pointer to the parent model and therefore its material, so uniforms
//can be set from here with: parentModel->getMaterialSlots()->at(0)->getShader()-><SETTER FUNC>
//ALL RENDERING IS HANDLED BY THE ENGINE, THIS IS JUST INFORMATION THE ENGINE TAKES IN


namespace ActorComponent
{

	class RenderComponent
	{
	private:
		unsigned long ptrLocation; //location of model instance pointer in engine vector. Should be unique for all render components
		Renderer::ModelInstance object;
		Renderer::Transformation transformation;
		const Renderer::Model* parentModelPtr;
		Engine* enginePtr;

	public:
		RenderComponent(const Renderer::Model* parentModel, Engine* engine, Renderer::Transformation transformation);
		~RenderComponent();

		void transform(Renderer::Transformation transformation);
		void replaceModel(const Renderer::Model* parentModel);
	};

}
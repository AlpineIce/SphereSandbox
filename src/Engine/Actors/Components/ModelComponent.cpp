#include "ModelComponent.h"

namespace ActorComponent
{

	RenderComponent::RenderComponent(const Renderer::Model* parentModel, Engine* engine, Renderer::Transformation transformation)
		:object(parentModel), parentModelPtr(parentModel), enginePtr(engine), transformation(transformation)
	{
		ptrLocation = enginePtr->addModelInstPtr(&object);
	}

	RenderComponent::~RenderComponent()
	{
		enginePtr->removeModelInstPtr(ptrLocation);
	}

	void RenderComponent::transform(Renderer::Transformation transformation)
	{
		this->transformation = transformation;
		object.transformation = transformation;
	}
	void RenderComponent::replaceModel(const Renderer::Model* parentModel)
	{
		object = Renderer::ModelInstance(parentModel);
		object.transformation = transformation;
		enginePtr->changeModelInstPtr(&object, ptrLocation);
	}
}

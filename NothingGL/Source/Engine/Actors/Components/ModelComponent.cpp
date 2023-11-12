#include "ModelComponent.h"
#include <iostream>

namespace ActorComponent
{

	RenderComponent::RenderComponent(const Renderer::Model* parentModel, Engine* engine)
		:object(parentModel), parentModelPtr(parentModel), enginePtr(engine)
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
		object.setTransformation(transformation);
	}
}

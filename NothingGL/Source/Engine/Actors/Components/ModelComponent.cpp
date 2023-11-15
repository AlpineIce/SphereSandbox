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
		object.setTransformation(transformation);
	}
	void RenderComponent::replaceModel(const Renderer::Model* parentModel)
	{
		object = Renderer::ModelInstance(parentModel);
		object.setTransformation(transformation);			//there is a potential problem here in that replacement doesnt initialize
		enginePtr->changeModelInstPtr(&object, ptrLocation);//old values into the new object
	}
}

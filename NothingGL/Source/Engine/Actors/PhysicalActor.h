#pragma once
#include "Actor.h"
#include "Components/ModelComponent.h"

//Actor class for anything that has a render component
namespace Actor
{

	class PhysicalActor : public Actor
	{
	private:
		ActorComponent::RenderComponent renderObj;

	public:
		PhysicalActor(const Renderer::Model* parentModel, Engine* engine);
		virtual ~PhysicalActor() override;

		virtual void transform(Transformation transform) override;
	};

}
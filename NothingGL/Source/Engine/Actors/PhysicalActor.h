#pragma once
#include "Actor.h"
#include "Components/ModelComponent.h"
#include "Components/CollisionComponent.h"

//Actor class for anything that has a render and collision component
namespace Actor
{

	class PhysicalActor : public Actor
	{
	private:
		

	public:
		PhysicalActor(const Renderer::Model* parentModel, Engine* engine, ActorComponent::ColliderType physicsType);
		virtual ~PhysicalActor() override;

		//Components
		ActorComponent::RenderComponent renderObj;
		ActorComponent::CollisionComponent collision;

		//functions
		virtual void transform(Transformation transform) override;
	};

}
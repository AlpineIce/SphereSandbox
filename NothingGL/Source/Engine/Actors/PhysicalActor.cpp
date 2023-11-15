#include "PhysicalActor.h"

namespace Actor
{

	PhysicalActor::PhysicalActor(const Renderer::Model* parentModel, Engine* engine, ActorComponent::ColliderType physicsType)
		:Actor(), 
		renderObj(parentModel, engine, { transformation.location, transformation.rotation, transformation.scale }),
		collision(engine, physicsType)
	{
	}

	PhysicalActor::~PhysicalActor()
	{
	}

	void PhysicalActor::transform(Transformation transformation)
	{
		this->transformation = transformation;
		renderObj.transform({ transformation.location, transformation.rotation, transformation.scale }); //weird cast I guess
	}

}

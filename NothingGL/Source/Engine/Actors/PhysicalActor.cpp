#include "PhysicalActor.h"

namespace Actor
{

	PhysicalActor::PhysicalActor(const Renderer::Model* parentModel, Engine* engine, Physics::ColliderType physicsType, Physics::PhysicsShape colliderShape)
		:Actor(), 
		renderObj(parentModel, engine, { transformation.location, transformation.rotation, transformation.scale }),
		collision(engine, physicsType, colliderShape)
	{
	}

	PhysicalActor::~PhysicalActor()
	{
	}

	void PhysicalActor::transform(Transformation transformation)
	{
		this->transformation = transformation;
		
		//transform children
		renderObj.transform({ transformation.location, transformation.rotation, transformation.scale }); //weird cast I guess
		collision.transform({ transformation.location, transformation.rotation, transformation.scale });
	}

}

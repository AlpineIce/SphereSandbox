#include "PhysicalActor.h"

namespace Actor
{

	PhysicalActor::PhysicalActor(const Renderer::Model* parentModel, Engine* engine, Physics::ColliderType physicsType, Physics::PhysicsShape colliderShape)
		:Actor(), 
		renderObj(parentModel, engine, { transformation.location, transformation.rotation, transformation.scale }),
		collision(engine, physicsType, colliderShape, { transformation.location, transformation.rotation, glm::vec3(0.5f) })
	{
	}

	PhysicalActor::~PhysicalActor()
	{
	}

	void PhysicalActor::transform(Transformation transformation)
	{
		this->transformation = transformation;
		
		//transform children (weird cast too I guess)
		renderObj.transform({ transformation.location, transformation.rotation, transformation.scale });
		collision.transform({ transformation.location, transformation.rotation, glm::vec3(1.0f) });
	}

	void PhysicalActor::updatePhysics()
	{
		Physics::Transformation newTransform = collision.getObject()->transformation;

		this->transformation = transformation;
		renderObj.transform({ newTransform.location, newTransform.rotation, newTransform.scale });
	}

}

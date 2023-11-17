#include "CollisionComponent.h"

namespace ActorComponent
{

	CollisionComponent::CollisionComponent(Engine* engine, Physics::ColliderType type, Physics::PhysicsShape shape)
		:enginePtr(engine), shapeArgs({ glm::vec3(1.0, 1.0, 1.0) })
	{
		object.shape = shape;
		object.type = type;
		object.shapeArgs = &shapeArgs;
		setShape(shape);

		ptrLocation = enginePtr->addCollisionPtr(type, &object);
	}

	CollisionComponent::~CollisionComponent()
	{
		enginePtr->removeCollisionPtr(object.type, ptrLocation);
	}

	void CollisionComponent::transform(Physics::Transformation transformation)
	{
		object.transformation = transformation;
	}

}

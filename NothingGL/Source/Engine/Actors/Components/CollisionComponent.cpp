#include "CollisionComponent.h"
#include <iostream>

namespace ActorComponent
{

	CollisionComponent::CollisionComponent(Engine* engine, Physics::ColliderType type, Physics::PhysicsShape shape, Physics::Transformation transformation)
		:enginePtr(engine)
	{
		object.transformation = transformation;
		object.shape = shape;
		object.type = type;
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

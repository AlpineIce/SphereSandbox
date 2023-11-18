#pragma once
#include "Physics/Physics.h"
#include "Engine.h"

//COLLISION COMPONENT
//This class is a component for objects that interact with each other, which
//can include collider physics, or just overlap events

namespace ActorComponent
{

	class CollisionComponent
	{
	private:
		Physics::PhysicsObject object;
		Engine* enginePtr;
		unsigned long ptrLocation;


	public:
		CollisionComponent(Engine* engine, Physics::ColliderType type, Physics::PhysicsShape shape, Physics::Transformation transformation);
		~CollisionComponent();

		void transform(Physics::Transformation transformation);

		inline Physics::ColliderType getType() { return object.type; }
		inline Physics::PhysicsObject* getObject() { return &object; }

		inline void setType(Physics::ColliderType type) { object.type = type; }
		inline void setComplexShape(std::vector<glm::vec3>* complexShape) { object.complexShape = complexShape; }
		inline void setShape(Physics::PhysicsShape shape) { object.shape = shape; }
		

	};

}
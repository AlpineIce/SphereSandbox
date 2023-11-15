#pragma once
#include "Physics/Physics.h"
#include "Engine.h"

//COLLISION COMPONENT
//This class is a component for objects that interact with each other, which
//can include collider physics, or just overlap events

namespace ActorComponent
{
	enum ColliderType
	{
		CONSTRAINT = 0,
		DYNAMIC = 1,
		OVERLAP	= 2		//for overlap events, such as a hitbox
	};

	enum ColliderShape
	{
		Sphere = 0,
		RectPrism,		//rectangular prism, or a cube if the lengths are similar
		Cylinder,
		Capsule,
		Mesh			//ue style complex collision
	};

	class CollisionComponent
	{
	private:
		ColliderType type;
		ColliderShape shape;

		Engine* enginePtr;

	public:
		CollisionComponent(Engine* engine, ColliderType type);
		~CollisionComponent();

		inline ColliderType getType() { return type; }
		inline void setType(ColliderType type) { this->type = type; }

		//im scared to learn how to implement this whole thing btw 
	};

}
#pragma once
#include "Engine.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

//Base class of all other actors, as well as the most basic form of any actor 
//Actor components can be added by including the header file and adding the object to the class


namespace Actor
{
	struct Transformation
	{
		glm::vec3 location = { 0.0f, 0.0f, 0.0f };
		glm::quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
	};

	class Actor
	{
	protected:
		Transformation transformation;
		Engine* engine;
		
	public:
		Actor(Engine* engine);
		virtual ~Actor();

		virtual void updateComponents(); //this doesnt actually do anything for a base class
		virtual void transform(Transformation transform);
	};

}
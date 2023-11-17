#pragma once
#include "GLM/glm.hpp"
#include "GLM/gtc/quaternion.hpp"

#include <vector>

namespace Physics
{
	struct PhysicsPrimitive //base struct
	{
		glm::vec3 center;
		float sizeX;
	};

	struct Sphere : PhysicsPrimitive
	{
		//sizeX in base struct is radius here
	};

	struct Plane : PhysicsPrimitive
	{
		float sizeY; //another dimension for 2D
	};

	struct RectPrism : Plane
	{
		float sizeZ; //yet another dimension for 3D
	};

}
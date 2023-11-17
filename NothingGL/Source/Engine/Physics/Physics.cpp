#include "Physics.h"

#include "GLFW/glfw3.h"

#include <thread>
#include <iostream>

namespace Physics
{
	
	Physics::PhysicsEngine::PhysicsEngine()
		:looping(true)
	{
	}

	Physics::PhysicsEngine::~PhysicsEngine()
	{
	}

	void PhysicsEngine::calculatePhysics(std::vector<PhysicsObject*>* dynamics,
										 std::vector<PhysicsObject*>* constraints,
										 std::vector<PhysicsObject*>* overlaps,
										 double deltaTime)
	{
		//dynamics on dynamics first
		for (PhysicsObject* objectA : *dynamics)
		{
			for (PhysicsObject* objectB : *dynamics)
			{
				if (objectA == objectB) //don't iterate on self
				{
					continue;
				}
				if (true) {} //make sure objects have potential to actually hit each other
				switch (objectA->shape)
				{
				case PhysicsShape::SPHERE:
					calcSphere(objectA);
					break;

				case PhysicsShape::PLANE:

					break;

				case PhysicsShape::RECTPRISM:

					break;

				case PhysicsShape::CYLINDER:

					break;

				case PhysicsShape::CAPSULE:

					break;

				case PhysicsShape::MESH:

					break;

				}
			}
		}

		//dynamics on constraints

		//overlaps
	}

	void PhysicsEngine::initLoop(std::vector<PhysicsObject*>* dynamics,
								 std::vector<PhysicsObject*>* constraints,
								 std::vector<PhysicsObject*>* overlaps)
	{
		if (!looping) { looping = true; }

		std::cout << "physics started" << std::endl;
		double last = glfwGetTime(); 
		double deltaTime = 0.0f;
		while (looping)
		{
			calculatePhysics(dynamics, constraints, overlaps, deltaTime);

			//update deltaTime
			double curTime = glfwGetTime(); //should be thread safe
			deltaTime = curTime - last;
			last = curTime;
		}
	}

	void PhysicsEngine::calcSphere(PhysicsObject* object)
	{
	}
}


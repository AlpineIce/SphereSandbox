#include "Physics.h"

#include "GLFW/glfw3.h"

#include <thread>
#include <chrono>
#include <iostream>

namespace Physics
{
	
	Physics::PhysicsEngine::PhysicsEngine(std::mutex* threadLock)
		:looping(true), threadLock(threadLock)
	{
	}

	Physics::PhysicsEngine::~PhysicsEngine()
	{
	}

	//----------LOOP AND "MAIN"----------//

	void PhysicsEngine::initLoop(std::vector<PhysicsObject*>* dynamics,
		std::vector<PhysicsObject*>* constraints,
		std::vector<PhysicsObject*>* overlaps)
	{
		if (!looping) { looping = true; } //for reinitiation of loop which may or may not happen

		double last = glfwGetTime();
		double deltaTime = 0.0f;
		while (looping)
		{
			if (deltaTime < 0.0001) //make sure physics isnt running so fast that it outdoes double precision, minimum time of 0.1ms
			{
				std::chrono::milliseconds waitTime(1);
				std::this_thread::sleep_for(waitTime);
			}

			calculatePhysics(dynamics, constraints, overlaps, deltaTime);

			//update deltaTime
			double curTime = glfwGetTime(); //should be thread safe
			deltaTime = curTime - last;
			last = curTime;
		}
	}

	void PhysicsEngine::calculatePhysics(std::vector<PhysicsObject*>* dynamics,
		std::vector<PhysicsObject*>* constraints,
		std::vector<PhysicsObject*>* overlaps,
		double deltaTime)
	{
		//calculate acceleration and angular velocity for all dynamic objects
		for (PhysicsObject* object : *dynamics)
		{
			threadLock->lock();
			solveAccel(object, deltaTime);
			solveRotation(object, deltaTime);
			threadLock->unlock();
		}

		//dynamics on static							//TODO RESEARCH AN ALGORITHM THAT ISNT O(N^2) (i am not proud of this code at all)
		for (PhysicsObject* dynamic : *dynamics)
		{
			for (PhysicsObject* constraint : *constraints)
			{
				solveDynamicStatic(dynamic, constraint, deltaTime);
			}
		}

		//dynamics on dynamics
		for (PhysicsObject* dynamicA : *dynamics)		//NESTED FOR LOOP MAKE CPU GO BRRRRRRRR
		{
			for (PhysicsObject* dynamicB : *dynamics)
			{
				solveDynamicDynamic(dynamicA, dynamicB, deltaTime);
			}
		}

		//overlaps
	}

	//----------SOLVER FUNCTIONS----------//

	void PhysicsEngine::solveAccel(PhysicsObject* object, double deltaTime)
	{
		if (object)
		{
			object->energy.velocity += (object->energy.acceleration + glm::dvec3(0.0, GRAVITY, 0.0)) * deltaTime * 0.5;
			object->transformation.location += object->energy.velocity * deltaTime;
			object->energy.velocity += (object->energy.acceleration + glm::dvec3(0.0, GRAVITY, 0.0)) * deltaTime * 0.5;
		}
	}

	void PhysicsEngine::solveRotation(PhysicsObject* object, double deltaTime)
	{
		object->transformation.rotation += (float)(deltaTime / 2.0) * glm::quat(0.0f, object->energy.rotationalVelocity) * object->transformation.rotation;
		object->transformation.rotation = glm::normalize(object->transformation.rotation);
	}

	void PhysicsEngine::solveDynamicStatic(PhysicsObject* dynamic, PhysicsObject* constraint, double deltaTime)
	{
		threadLock->lock();

		//check if objects can potentially hit each other first
		glm::vec3 distVector = dynamic->transformation.location - constraint->transformation.location;
		glm::vec3 dS = dynamic->transformation.scale;		//dynamic scale
		glm::vec3 cS = constraint->transformation.scale;	//constraint scale
		float distOffset = glm::max(glm::max(dS.x, dS.y), dS.z) + glm::max(glm::max(cS.x, cS.y), cS.z);

		//some other stuff
		PhysicsShape dynamicShape = dynamic->shape;
		PhysicsShape constraintShape = constraint->shape;
		threadLock->unlock();
		
		float intrusion = glm::length(distVector) - distOffset;
		if (intrusion < 0.0) //objects in physics range
		{
			threadLock->lock();
			EnergyConservation newEnergy;
			//newEnergy.acceleration = dynamic->energy.acceleration;
			newEnergy = dynamic->energy;

			switch (dynamicShape)
			{
			case PhysicsShape::SPHERE:
				if (constraintShape == PhysicsShape::SPHERE)
				{
				}

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

			//dynamic->energy = newEnergy;
			threadLock->unlock();
		}

	}

	void PhysicsEngine::solveDynamicDynamic(PhysicsObject* dynamicObj1, PhysicsObject* dynamicObj2, double deltaTime)
	{
		if (dynamicObj1 == dynamicObj2) //i just didnt want to add another nested block to the function that calls this btw
		{
			return;
		}

		//kys for making this
	}

	//TODO OVERLAP SOLVER

}


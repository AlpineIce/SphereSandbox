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

	void PhysicsEngine::initLoop(
		std::map<unsigned long, PhysicsObject*>* dynamics)
	{
		if (!looping) { looping = true; } //for reinitiation of loop which may or may not happen

		double last = glfwGetTime();
		double deltaTime = 0.0f;
		while (looping)
		{
			if (deltaTime < 0.001) //make sure physics isnt running so fast that it outdoes double precision, minimum time of 1ms
			{
				std::chrono::milliseconds waitTime(1);
				std::this_thread::sleep_for(waitTime);
			}

			calculatePhysics(dynamics, deltaTime);

			//update deltaTime
			double curTime = glfwGetTime(); //should be thread safe
			deltaTime = curTime - last;
			last = curTime;
		}
	}

	void PhysicsEngine::calculatePhysics(
		std::map<unsigned long, PhysicsObject*>* dynamics, double deltaTime)
	{
		//calculate acceleration and angular velocity for all dynamic objects
		for (auto const& [key, object] : *dynamics)
		{
			
			solveAccel(object, deltaTime);
			solveRotation(object, deltaTime);
			
		}

		//dynamics on dynamics
		for (unsigned long i = 0; i < dynamics->size(); i++) //TODO IMPLEMENT BOUNDING VOLUME HIERARCHIES 
		{
			for (unsigned long j = i + 1; j < dynamics->size(); j++)
			{
				solveDynamicDynamic(dynamics->at(i), dynamics->at(j), deltaTime);
			}
		}
	}

	//----------SOLVER FUNCTIONS----------//

	void PhysicsEngine::solveAccel(PhysicsObject* object, double deltaTime)
	{
		threadLock->lock();
		if (object)
		{
			object->energy.velocity += (object->energy.acceleration + glm::dvec3(0.0, GRAVITY, 0.0)) * deltaTime * 0.5;
			if (glm::length(object->transformation.location) > 0.0f)
			{
				glm::dvec3 towardCenter = glm::normalize(-object->transformation.location);
				object->energy.velocity += (object->energy.acceleration + (towardCenter * 4.0)) * deltaTime * 0.5;
				object->energy.velocity += (object->energy.acceleration + (towardCenter * 4.0)) * deltaTime * 0.5;
			}

			object->transformation.location += object->energy.velocity * deltaTime;

			object->energy.velocity += (object->energy.acceleration + glm::dvec3(0.0, GRAVITY, 0.0)) * deltaTime * 0.5;
			if (glm::length(object->transformation.location) > 0.0f)
			{
				glm::dvec3 towardCenter = glm::normalize(-object->transformation.location);
				object->energy.velocity += (object->energy.acceleration + (towardCenter * 4.0)) * deltaTime * 0.5;
				object->energy.velocity += (object->energy.acceleration + (towardCenter * 4.0)) * deltaTime * 0.5;
			}
		}
		threadLock->unlock();
	}

	void PhysicsEngine::solveRotation(PhysicsObject* object, double deltaTime)
	{
		threadLock->lock();
		object->transformation.rotation += (deltaTime / 2.0) * glm::dquat(0.0, object->energy.rotationalVelocity) * object->transformation.rotation;
		object->transformation.rotation = glm::normalize(object->transformation.rotation);
		threadLock->unlock();
	}

	void PhysicsEngine::solveDynamicDynamic(PhysicsObject* dynamicObj1, PhysicsObject* dynamicObj2, double deltaTime)
	{
		threadLock->lock();
		if (dynamicObj1 == dynamicObj2) //i just didnt want to add another nested block to the function that calls this btw
		{
			threadLock->unlock();
			return;
		}

		//check if objects can potentially hit each other first
		glm::dvec3 distVector = dynamicObj1->transformation.location - dynamicObj2->transformation.location;
		glm::vec3 do1 = dynamicObj1->transformation.scale;		//dynamic obj 1 scale
		glm::vec3 do2 = dynamicObj2->transformation.scale;	//dynamic obj 2 scale

		float distOffset = glm::max(glm::max(do1.x, do1.y), do1.z) + glm::max(glm::max(do2.x, do2.y), do2.z);
		float length = (float)glm::length(distVector);
		
		if (length - distOffset < 0.0) //objects in physics range
		{
			EnergyConservation energy1 = dynamicObj1->energy;
			EnergyConservation energy2 = dynamicObj2->energy;
			switch (dynamicObj1->shape)
			{
			case PhysicsShape::SPHERE:
				if (dynamicObj2->shape == PhysicsShape::SPHERE)
				{
					//object 1
					float massOfset = 2.0f * energy1.mass / (energy1.mass + energy2.mass);
					double numerator = glm::dot(energy1.velocity - energy2.velocity, distVector);
					float denominator = length * length;

					glm::dvec3 newOffset = (massOfset * (numerator / denominator) * distVector);
					dynamicObj1->energy.velocity = dynamicObj1->energy.velocity - newOffset;
					
					//object 2 (shadow previous variables i guess)
					massOfset = 2.0f * energy2.mass / (energy1.mass + energy2.mass);
					numerator = glm::dot(energy2.velocity - energy1.velocity, -distVector);
					denominator = length * length;

					newOffset = (massOfset * (numerator / denominator) * -distVector);
					dynamicObj2->energy.velocity = dynamicObj2->energy.velocity - newOffset;

					//move objects to outside so they dont just blob together
					dynamicObj1->transformation.location -= (double)(length - distOffset) * glm::normalize(distVector) * 0.5;
					dynamicObj2->transformation.location += (double)(length - distOffset) * glm::normalize(distVector) * 0.5;
				}

				break;
			}
		}
		threadLock->unlock();
	}

}


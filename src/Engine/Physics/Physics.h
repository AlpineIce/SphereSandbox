#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include <map>
#include <vector>
#include <mutex>

namespace Physics
{

	struct Transformation
	{
		glm::dvec3 location = glm::dvec3(0.0);
		glm::dquat rotation = { 1.0, 0.0, 0.0, 0.0 };
		glm::dvec3 scale = glm::dvec3(0.5);
	};

	struct EnergyConservation
	{
		glm::dvec3 velocity = glm::dvec3(0.0);
		glm::vec3 rotationalVelocity = glm::vec3(0.0f);
		glm::dvec3 acceleration = glm::dvec3(0.0);
		float mass = 1.0f;
	};

	struct MaterialProperties
	{
		float friction = 0.1f;
	};

	enum ColliderType
	{
		DYNAMIC = 0	//for overlap events, such as a hitbox
	};

	enum PhysicsShape
	{
		SPHERE = 0,
		BOX = 2			//rectangular prism, or a cube if the lengths are similar
	};

	struct PhysicsObject
	{
		ColliderType type = DYNAMIC;
		PhysicsShape shape = SPHERE;

		Transformation transformation;
		EnergyConservation energy;
		MaterialProperties matProperties;
	};

	class PhysicsEngine
	{
	private:
		const double GRAVITY = -0.0;
		bool looping;
		std::mutex* threadLock;

		void calculatePhysics(
			std::map<unsigned long, PhysicsObject*>* dynamics, double deltaTime);

		void solveAccel(PhysicsObject* object, double deltaTime);
		void solveRotation(PhysicsObject* object, double deltaTime);
		void solveDynamicDynamic(PhysicsObject* dynamicObj1, PhysicsObject* dynamicObj2, double deltaTime);
		
	public:
		PhysicsEngine(std::mutex* threadLock);
		~PhysicsEngine();

		void initLoop(std::map<unsigned long, PhysicsObject*>* dynamics);

		inline void stopLoop() { looping = false; }
	};

}

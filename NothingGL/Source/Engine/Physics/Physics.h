#pragma once
#include "GLM/glm.hpp"
#include "GLM/gtc/quaternion.hpp"

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
		glm::vec3 rotationalVelocity = glm::vec3(1.0f, 7.0f, 0.0f);
		glm::dvec3 acceleration = glm::dvec3(0.0);
		float mass = 1.0f;
	};

	enum ColliderType
	{
		STATIC = 0,
		DYNAMIC = 1,
		OVERLAP = 2		//for overlap events, such as a hitbox
	};

	enum PhysicsShape
	{
		SPHERE = 0,
		PLANE = 1,
		BOX = 2,		//rectangular prism, or a cube if the lengths are similar
		CYLINDER = 3,
		CAPSULE = 4,
		MESH = 5			//ue style complex collision
	};

	struct PhysicsObject
	{
		ColliderType type = STATIC;
		PhysicsShape shape = SPHERE;

		Transformation transformation;
		EnergyConservation energy;

		std::vector<glm::vec3>* complexShape;
	};

	class PhysicsEngine
	{
	private:
		const double GRAVITY = -0.0;
		bool looping;
		std::mutex* threadLock;

		void calculatePhysics(
			std::map<unsigned long, PhysicsObject*>* dynamics, //function uses solvers below
			std::map<unsigned long, PhysicsObject*>* constraints,
			std::map<unsigned long, PhysicsObject*>* overlaps,
			double deltaTime);

		void solveAccel(PhysicsObject* object, double deltaTime);
		void solveRotation(PhysicsObject* object, double deltaTime);
		void solveDynamicStatic(PhysicsObject* dynamicObj, PhysicsObject* staticObj, double deltaTime);
		void solveDynamicDynamic(PhysicsObject* dynamicObj1, PhysicsObject* dynamicObj2, double deltaTime);

		float findIntrusion(PhysicsObject* obj1, PhysicsObject* obj2);
		
	public:
		PhysicsEngine(std::mutex* threadLock);
		~PhysicsEngine();

		void initLoop(std::map<unsigned long, PhysicsObject*>* dynamics,
					  std::map<unsigned long, PhysicsObject*>* constraints,
					  std::map<unsigned long, PhysicsObject*>* overlaps);

		inline void stopLoop() { looping = false; }
	};

}

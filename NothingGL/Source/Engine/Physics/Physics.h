#pragma once
#include "GLM/glm.hpp"
#include "GLM/gtc/quaternion.hpp"

#include <vector>
#include <mutex>

namespace Physics
{

	struct Transformation
	{
		glm::vec3 location = { 0.0f, 0.0f, 0.0f };
		glm::quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
	};

	struct EnergyConservation
	{
		glm::dvec3 velocity = glm::dvec3(0.0f);
		glm::vec3 rotationalVelocity = glm::vec3(1.0f, 7.0f, 0.0f);
		glm::dvec3 acceleration = glm::dvec3(0.0f);
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
		RECTPRISM = 2,		//rectangular prism, or a cube if the lengths are similar
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

	class PhysicsEngine		//i currently have no idea what to put here btw
	{
	private:
		const double GRAVITY = 0.0;
		bool looping;
		std::mutex* threadLock;

		void calculatePhysics(std::vector<PhysicsObject*>* dynamics, //function uses solvers below
			std::vector<PhysicsObject*>* constraints,
			std::vector<PhysicsObject*>* overlaps,
			double deltaTime);
		void solveAccel(PhysicsObject* object, double deltaTime);
		void solveRotation(PhysicsObject* object, double deltaTime);
		void solveDynamicStatic(PhysicsObject* dynamicObj, PhysicsObject* staticObj, double deltaTime);
		void solveDynamicDynamic(PhysicsObject* dynamicObj1, PhysicsObject* dynamicObj2, double deltaTime);
		
	public:
		PhysicsEngine(std::mutex* threadLock);
		~PhysicsEngine();

		void initLoop(std::vector<PhysicsObject*>* dynamics,
					  std::vector<PhysicsObject*>* constraints,
					  std::vector<PhysicsObject*>* overlaps);

		inline void stopLoop() { looping = false; }
	};

}

#pragma once
#include "Shapes.h"

namespace Physics
{

	struct Transformation
	{
		glm::vec3 location = { 0.0f, 0.0f, 0.0f };
		glm::quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
	};

	enum ColliderType
	{
		CONSTRAINT = 0,
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

	struct ShapeArgs //optional variables dependent on shape
	{
		glm::vec3 size;
	};

	struct PhysicsObject
	{
		ColliderType type;
		Transformation transformation;
		glm::vec3 velocity;
		float mass;

		ShapeArgs* shapeArgs;
		PhysicsShape shape;
		std::vector<glm::vec3>* complexShape;
	};

	class PhysicsEngine		//i currently have no idea what to put here btw
	{
	private:
		const float GRAVITY = 9.81f;
		bool looping;
		void calcSphere(PhysicsObject* object);
	public:
		PhysicsEngine();
		~PhysicsEngine();

		void calculatePhysics(std::vector<PhysicsObject*>* dynamics,
							  std::vector<PhysicsObject*>* constraints,
							  std::vector<PhysicsObject*>* overlaps,
							  double deltaTime);

		void initLoop(std::vector<PhysicsObject*>* dynamics,
					  std::vector<PhysicsObject*>* constraints,
					  std::vector<PhysicsObject*>* overlaps);

		void stopLoop() { looping = false; }
	};

}

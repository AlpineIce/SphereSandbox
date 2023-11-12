#include "PhysicalActor.h"

namespace Actor
{

	PhysicalActor::PhysicalActor(const Renderer::Model* parentModel, Engine* engine)
		:Actor(), renderObj(parentModel, engine)
	{
	}

	PhysicalActor::~PhysicalActor()
	{
	}

	void PhysicalActor::transform(Transformation transformation)
	{
		this->transformation = transformation;
		renderObj.transform({ transformation.location, transformation.rotation, transformation.scale }); //weird cast I guess
	}

}

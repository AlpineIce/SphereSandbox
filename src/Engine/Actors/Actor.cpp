#include "Actor.h"

namespace Actor
{

	Actor::Actor()
	{
		transformation.location = { 0.0f, 0.0f, 0.0f };
		transformation.rotation = { 1.0f, 0.0f, 0.0f, 0.0f }; //quaternions are scary
		transformation.scale = { 1.0f, 1.0f, 1.0f };
	}

	Actor::~Actor()
	{
	}

	void Actor::updateComponents()
	{
	}

	void Actor::transform(Transformation transformation)
	{
		this->transformation = transformation;
	}

}

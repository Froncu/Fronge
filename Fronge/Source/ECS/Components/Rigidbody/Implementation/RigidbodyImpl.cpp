#include "froch.hpp"

#include "RigidbodyImpl.hpp"

namespace fro
{
	b2World& Rigidbody::Implementation::getb2World()
	{
		static b2World sWorld{ b2Vec2{ 0.0f, 9.81f } };
		return sWorld;
	}

	Rigidbody::Implementation::Implementation(b2BodyDef const bodyDefinition)
		: mBody
		{
			getb2World().CreateBody(&bodyDefinition),
			std::bind(&b2World::DestroyBody, &getb2World(), std::placeholders::_1)
		}
	{
	}

	b2Body& Rigidbody::Implementation::getb2Body() const
	{
		return *mBody;
	}

	b2BodyDef Rigidbody::Implementation::getb2BodyDef() const
	{
		b2BodyDef bodyDefinition{};
		bodyDefinition.type = mBody->GetType();
		bodyDefinition.position = mBody->GetPosition();
		bodyDefinition.angle = mBody->GetAngle();
		bodyDefinition.linearVelocity = mBody->GetLinearVelocity();
		bodyDefinition.angularVelocity = mBody->GetAngularVelocity();
		bodyDefinition.linearDamping = mBody->GetLinearDamping();
		bodyDefinition.angularDamping = mBody->GetAngularDamping();
		bodyDefinition.allowSleep = mBody->IsSleepingAllowed();
		bodyDefinition.awake = mBody->IsAwake();
		bodyDefinition.fixedRotation = mBody->IsFixedRotation();
		bodyDefinition.bullet = mBody->IsBullet();
		bodyDefinition.enabled = mBody->IsEnabled();
		bodyDefinition.gravityScale = mBody->GetGravityScale();

		return bodyDefinition;
	}
}
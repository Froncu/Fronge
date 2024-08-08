#include "froch.hpp"

#include "ECS/Entity/Entity.hpp"
#include "ECS/Components/Transform/Transform.hpp"
#include "Implementation/ColliderImpl.hpp"
#include "Implementation/RigidbodyImpl.hpp"
#include "Rigidbody.hpp"
#include "Utility/VariantVisitor.hpp"

namespace fro
{
	Rigidbody::Rigidbody()
		: mImplementation{ std::make_unique<Implementation>() }
	{
	}

	Rigidbody::Rigidbody(Rigidbody const& other)
		: Component(other)

		, mImplementation{ std::make_unique<Implementation>(other.mImplementation->getb2BodyDef()) }
	{
		mColliders.clear();
		for (auto const& otherCollider : other.mColliders)
			mColliders.push_back(std::make_unique<Collider::Implementation>(mImplementation->getb2Body(),
				otherCollider.getImplementation().getb2FixtureDef()));
	}

	Rigidbody::Rigidbody(Rigidbody&& other) noexcept
		: Component(std::move(other))

		, mImplementation{ std::move(other.mImplementation) }
		, mColliders{ std::move(other.mColliders) }
	{
	}

	Rigidbody::~Rigidbody()
	{
	}

	Rigidbody& Rigidbody::operator=(Rigidbody const& other)
	{
		if (this == &other)
			return *this;

		Component::operator=(other);

		mImplementation = std::make_unique<Implementation>(other.mImplementation->getb2BodyDef());

		mColliders.clear();
		for (auto const& otherCollider : other.mColliders)
			mColliders.push_back(std::make_unique<Collider::Implementation>(mImplementation->getb2Body(),
				otherCollider.getImplementation().getb2FixtureDef()));

		return *this;
	}

	Rigidbody& Rigidbody::operator=(Rigidbody&& other) noexcept
	{
		if (this == &other)
			return *this;

		Component::operator=(std::move(other));

		mImplementation = std::move(other.mImplementation);
		mColliders = std::move(other.mColliders);

		return *this;
	}

	Rigidbody::Implementation& Rigidbody::getImplementation() const
	{
		return *mImplementation;
	}

	void Rigidbody::setType(Type const type)
	{
		switch (type)
		{
		case Type::STATIC:
			mImplementation->getb2Body().SetType(b2BodyType::b2_staticBody);
			break;

		case Type::KINEMATIC:
			mImplementation->getb2Body().SetType(b2BodyType::b2_kinematicBody);
			break;

		case Type::DYNAMIC:
			mImplementation->getb2Body().SetType(b2BodyType::b2_dynamicBody);
			break;
		}
	}

	void Rigidbody::setFixedRotation(bool const lockRotation)
	{
		mImplementation->getb2Body().SetFixedRotation(lockRotation);
	}

	void Rigidbody::setLinearVelocity(Vector2<double> const velocity)
	{
		mImplementation->getb2Body().SetLinearVelocity({ static_cast<float>(velocity.x), static_cast<float>(velocity.y) });
	}

	void Rigidbody::setAngularVelocity(double const velocity)
	{
		mImplementation->getb2Body().SetAngularVelocity(static_cast<float>(velocity));
	}

	void Rigidbody::applyForce(Vector2<double> const force)
	{
		mImplementation->getb2Body().ApplyForceToCenter({ static_cast<float>(force.x), static_cast<float>(force.y) }, true);
	}

	void Rigidbody::applyTorque(double const torque)
	{
		mImplementation->getb2Body().ApplyTorque(static_cast<float>(torque), true);
	}

	void Rigidbody::applyLinearImpulse(Vector2<double> const impulse)
	{
		mImplementation->getb2Body().ApplyLinearImpulseToCenter({ static_cast<float>(impulse.x), static_cast<float>(impulse.y) }, true);
	}

	void Rigidbody::applyAngularImpulse(double const impulse)
	{
		mImplementation->getb2Body().ApplyAngularImpulse(static_cast<float>(impulse), true);
	}

	Rigidbody::Type Rigidbody::getType() const
	{
		switch (mImplementation->getb2Body().GetType())
		{
		case b2_staticBody:
			return Type::STATIC;

		case b2_kinematicBody:
			return Type::KINEMATIC;

		case b2_dynamicBody:
			return Type::DYNAMIC;
		}
	}

	bool Rigidbody::isFixedRotation() const
	{
		return mImplementation->getb2Body().IsFixedRotation();
	}

	Vector2<double> Rigidbody::getLinearVelocity() const
	{
		auto const linearVelocity{ mImplementation->getb2Body().GetLinearVelocity() };
		return { static_cast<double>(linearVelocity.x), static_cast<double>(linearVelocity.y) };
	}

	double Rigidbody::getAngularVelocity() const
	{
		return static_cast<double>(mImplementation->getb2Body().GetAngularVelocity());
	}

	Collider& Rigidbody::addCollider()
	{
		auto const b2Shape{ Collider::Implementation::createb2Shape(Rectangle<double>{ 0.0, 0.0, 32.0, 32.0 }) };
		b2FixtureDef fixtureDefinition{};
		fixtureDefinition.shape = b2Shape.get();

		return mColliders.emplace_back(std::make_unique<Collider::Implementation>(mImplementation->getb2Body(), fixtureDefinition));
	}
}
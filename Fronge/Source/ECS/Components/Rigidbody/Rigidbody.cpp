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

	Collider& Rigidbody::addCollider()
	{
		auto const b2Shape{ Collider::Implementation::createb2Shape(Rectangle<double>{ 0.0, 0.0, 32.0, 32.0 }) };
		b2FixtureDef fixtureDefinition{};
		fixtureDefinition.shape = b2Shape.get();

		return mColliders.emplace_back(std::make_unique<Collider::Implementation>(mImplementation->getb2Body(), fixtureDefinition));
	}
}
#include "froch.hpp"

#include "ECS/Entity/Entity.hpp"
#include "ECS/Components/Transform/Transform.hpp"
#include "Implementation/RigidbodyImpl.hpp"
#include "Rigidbody.hpp"

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
	}

	Rigidbody::Rigidbody(Rigidbody&& other) noexcept
		: Component(std::move(other))

		, mImplementation{ std::move(other.mImplementation) }
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

		return *this;
	}

	Rigidbody& Rigidbody::operator=(Rigidbody&& other) noexcept
	{
		if (this == &other)
			return *this;

		Component::operator=(std::move(other));

		mImplementation = std::move(other.mImplementation);

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
}
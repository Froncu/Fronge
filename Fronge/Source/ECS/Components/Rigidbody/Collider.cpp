#include "froch.hpp"

#include "Collider.hpp"
#include "Implementation/ColliderImpl.hpp"
#include "Implementation/RigidbodyImpl.hpp"

namespace fro
{
	Collider::Collider(std::unique_ptr<Implementation>&& implementation)
		: mImplementation{ std::move(implementation) }
	{
	}

	Collider::Collider(Collider const& other)
		: Referencable(other)

		, mImplementation
		{
			std::make_unique<Implementation>(*other.mImplementation->getb2Fixture().GetBody(),
				other.mImplementation->getb2FixtureDef())
		}
	{
	}

	Collider::Collider(Collider&& other) noexcept
		: Referencable(std::move(other))

		, mImplementation{ std::move(other.mImplementation) }
	{
	}

	Collider::~Collider()
	{
	}

	Collider& Collider::operator=(Collider const& other)
	{
		if (this == &other)
			return *this;

		Referencable::operator=(other);

		mImplementation =
			std::make_unique<Implementation>(*other.mImplementation->getb2Fixture().GetBody(),
				other.mImplementation->getb2FixtureDef());

		return *this;
	}

	Collider& Collider::operator=(Collider&& other) noexcept
	{
		if (this == &other)
			return *this;

		Referencable::operator=(std::move(other));

		mImplementation = std::move(other.mImplementation);

		return *this;
	}

	Collider::Implementation& Collider::getImplementation() const
	{
		return *mImplementation;
	}

	void Collider::setShape(std::variant<Rectangle<double>, Polygon<double>, Chain<double>, Circle<double>> const& shape)
	{
		auto const b2Shape{ Collider::Implementation::createb2Shape(shape) };
		b2FixtureDef fixtureDefinition{ mImplementation->getb2FixtureDef() };
		fixtureDefinition.shape = b2Shape.get();

		mImplementation = std::make_unique<Implementation>(*mImplementation->getb2Fixture().GetBody(), fixtureDefinition);
	}

	void Collider::setDensity(double const density)
	{
		mImplementation->getb2Fixture().SetDensity(static_cast<float>(density));
		mImplementation->getb2Fixture().GetBody()->ResetMassData();
	}

	void Collider::setRestitution(double const restitution)
	{
		mImplementation->getb2Fixture().SetRestitution(static_cast<float>(restitution));
	}
}
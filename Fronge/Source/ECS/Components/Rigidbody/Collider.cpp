#include "froch.hpp"

#include "Collider.hpp"
#include "Implementation/ColliderImpl.hpp"
#include "Implementation/RigidbodyImpl.hpp"

namespace fro
{
	Collider::Collider(std::unique_ptr<Implementation>&& implementation)
		: mImplementation{ std::move(implementation) }
	{
		mImplementation->getb2Fixture().GetUserData().pointer = reinterpret_cast<std::uintptr_t>(this);
	}

	Collider::Collider(Collider const& other)
		: Referencable(other)

		, mImplementation
		{
			std::make_unique<Implementation>(*other.mImplementation->getb2Fixture().GetBody(),
				other.mImplementation->getb2FixtureDef())
		}
	{
		mImplementation->getb2Fixture().GetUserData().pointer = reinterpret_cast<std::uintptr_t>(this);
	}

	Collider::Collider(Collider&& other) noexcept
		: Referencable(std::move(other))

		, mImplementation{ std::move(other.mImplementation) }
	{
		mImplementation->getb2Fixture().GetUserData().pointer = reinterpret_cast<std::uintptr_t>(this);
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

		mImplementation->getb2Fixture().GetUserData().pointer = reinterpret_cast<std::uintptr_t>(this);

		return *this;
	}

	Collider& Collider::operator=(Collider&& other) noexcept
	{
		if (this == &other)
			return *this;

		Referencable::operator=(std::move(other));

		mImplementation = std::move(other.mImplementation);

		mImplementation->getb2Fixture().GetUserData().pointer = reinterpret_cast<std::uintptr_t>(this);

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

	void Collider::setFriction(double const friction)
	{
		mImplementation->getb2Fixture().SetFriction(static_cast<float>(friction));
	}


	void Collider::setSensor(bool const isSensor)
	{
		mImplementation->getb2Fixture().SetSensor(isSensor);
	}

	void Collider::setCategoryBits(std::uint16_t const categoryBits)
	{
		b2Fixture& fixture{ mImplementation->getb2Fixture() };

		b2Filter const currentFilterData{ fixture.GetFilterData() };
		b2Filter newFilterData{};
		newFilterData.categoryBits = categoryBits;
		newFilterData.maskBits = currentFilterData.maskBits;
		newFilterData.groupIndex = currentFilterData.groupIndex;

		fixture.SetFilterData(newFilterData);
	}

	void Collider::setMaskBits(std::uint16_t const maskBits)
	{
		b2Fixture& fixture{ mImplementation->getb2Fixture() };

		b2Filter const currentFilterData{ fixture.GetFilterData() };
		b2Filter newFilterData{};
		newFilterData.categoryBits = currentFilterData.categoryBits;
		newFilterData.maskBits = maskBits;
		newFilterData.groupIndex = currentFilterData.groupIndex;

		fixture.SetFilterData(newFilterData);
	}

	void Collider::setGroupIndex(std::int16_t const groupIndex)
	{
		b2Fixture& fixture{ mImplementation->getb2Fixture() };

		b2Filter const currentFilterData{ fixture.GetFilterData() };
		b2Filter newFilterData{};
		newFilterData.categoryBits = currentFilterData.categoryBits;
		newFilterData.maskBits = currentFilterData.maskBits;
		newFilterData.groupIndex = groupIndex;

		fixture.SetFilterData(newFilterData);
	}

	std::uint16_t Collider::getCategoryBits() const
	{
		return mImplementation->getb2Fixture().GetFilterData().categoryBits;
	}

	std::uint16_t Collider::getMaskBits() const
	{
		return mImplementation->getb2Fixture().GetFilterData().maskBits;
	}

	std::int16_t Collider::getGroupIndex() const
	{
		return mImplementation->getb2Fixture().GetFilterData().groupIndex;
	}
}
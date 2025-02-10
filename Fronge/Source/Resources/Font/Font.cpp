#include "froch.hpp"

#include "Font.hpp"
#include "Logger/Logger.hpp"
#include "Implementation/FontImpl.hpp"

namespace fro
{
	IDGenerator Font::sIDGenerator{};

	Font::Font(std::string_view const filePath, int const size)
		: mImplementation{ std::make_unique<Implementation>(filePath, size) }
	{
		Logger::info("loaded {} as Font with ID {}!",
			filePath, mID);
	}

	Font::Font(Font&& other) noexcept
		: Referencable(std::move(other))
		, mImplementation{ std::move(other.mImplementation) }
	{
	}

	Font::~Font()
	{
		Logger::info("destroyed Font with ID {}!",
			mID);
	}

	Font& Font::operator=(Font&& other) noexcept
	{
		if (this == &other)
			return *this;

		Referencable::operator=(std::move(other));

		mImplementation = std::move(other.mImplementation);

		return *this;
	}

	Font::Implementation& Font::getImplementation() const
	{
		return *mImplementation;
	}

	ID const& Font::getID() const
	{
		return mID;
	}
}
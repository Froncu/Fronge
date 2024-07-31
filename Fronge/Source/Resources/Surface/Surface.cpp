#include "froch.hpp"

#include "Implementation/SurfaceImpl.hpp"
#include "Surface.hpp"

#include <SDL_image.h>

namespace fro
{
	IDGenerator Surface::sIDGenerator{};

	Surface::Surface(std::string_view const imagePath)
		: mImplementation{ std::make_unique<Implementation>(imagePath) }
		, mSize{ mImplementation->getSDLSurface()->w, mImplementation->getSDLSurface()->h }
	{
		Logger::info("loaded {} as surface with ID {}!",
			imagePath, mID);
	}

	Surface::Surface(Font const& font, std::string_view const text)
		: mImplementation{ std::make_unique<Implementation>(font, text) }
		, mSize{ mImplementation->getSDLSurface()->w, mImplementation->getSDLSurface()->h }
	{
		Logger::info("loaded \"{}\" as Surface with ID {}!",
			text, mID);
	}

	Surface::Surface(Surface&& other) noexcept
		: Referencable(std::move(other))

		, mID{ std::move(other.mID) }
		, mImplementation{ std::move(other.mImplementation) }
		, mSize{ other.getSize() }
	{
		other.mSize = {};
	}

	Surface::~Surface()
	{
		Logger::info("destroyed Surface with ID {}!",
			mID);
	}

	Surface& Surface::operator=(Surface&& other) noexcept
	{
		if (this == &other)
			return *this;

		Referencable::operator=(std::move(other));

		mID = std::move(other.mID);
		mImplementation = std::move(other.mImplementation);
		mSize = other.getSize();

		other.mSize = {};

		return *this;
	}

	Surface::Implementation& Surface::getImplementation() const
	{
		return *mImplementation;
	}

	ID const& Surface::getID() const
	{
		return mID;
	}

	Vector2<int> Surface::getSize() const
	{
		return mSize;
	}
}
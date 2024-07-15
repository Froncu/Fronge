#if not defined SURFACE_HPP
#define SURFACE_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Idenitifer/IDGenerator.hpp"
#include "Maths/MathStructs.hpp"
#include "Reference/Reference.hpp"
#include "Resources/Font.hpp"

namespace fro
{
	class Renderer;

	class Surface final : public Referencable
	{
		class Implementation;

	public:
		FRO_API Surface(std::string_view const imagePath);
		FRO_API Surface(Font const& font, std::string_view const text);
		FRO_API Surface(Surface&& other) noexcept;

		FRO_API ~Surface();

		FRO_API Surface& operator=(Surface&& other) noexcept;

		FRO_API FRO_NODISCARD Implementation& getImplementation() const;

		FRO_API FRO_NODISCARD std::size_t getID() const;
		FRO_API FRO_NODISCARD Vector2<int> getSize() const;

	private:
		static IDGenerator sIDGenerator;

		Surface(Surface const&) = delete;

		Surface& operator=(Surface const&) = delete;

		ID mID{ sIDGenerator.get() };
		std::unique_ptr<Implementation> mImplementation;
		Vector2<int> mSize;
	};
}

#endif
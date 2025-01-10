#ifndef FONT_HPP
#define FONT_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Idenitifer/IDGenerator.hpp"
#include "Reference/Referencable.hpp"

namespace fro
{
	class Font final : public Referencable
	{
	public:
		class Implementation;

		FRO_API Font(std::string_view const filePath, int const size);
		FRO_API Font(Font&& other) noexcept;

		FRO_API ~Font();

		FRO_API Font& operator=(Font&& other) noexcept;

		FRO_API FRO_NODISCARD Implementation& getImplementation() const;

		FRO_API FRO_NODISCARD ID const& getID() const;

	private:
		static IDGenerator sIDGenerator;

		Font(Font const&) = delete;

		Font& operator=(Font const&) = delete;

		ID mID{ sIDGenerator.get() };
		std::unique_ptr<Implementation> mImplementation;
	};
}

#endif
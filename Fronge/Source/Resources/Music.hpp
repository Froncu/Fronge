#if not defined MUSIC_HPP
#define MUSIC_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Reference/Referencable.hpp"
#include "Idenitifer/IDGenerator.hpp"

namespace fro
{
	class Music final : public Referencable
	{
		class Implementation;

	public:
		FRO_API Music(std::string_view const filePath);
		FRO_API Music(Music&& other) noexcept;

		FRO_API ~Music();
		FRO_API Music& operator=(Music&& other) noexcept;

		FRO_API FRO_NODISCARD Implementation& getImplementation() const;

		FRO_API FRO_NODISCARD ID const& getID() const;

	private:
		static IDGenerator sIDGenerator;

		Music(Music const&) = delete;

		Music& operator=(Music const&) = delete;

		ID mID{ sIDGenerator.get() };
		std::unique_ptr<Implementation> mImplementation;
	};
}

#endif
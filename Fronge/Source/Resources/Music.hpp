#if not defined MUSIC_HPP
#define MUSIC_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Reference/Referencable.hpp"

namespace fro
{
	class Music final : public Referencable
	{
		class Implementation;

	public:
		struct Descriptor final
		{
			std::string filePath;
		};

		FRO_API Music(Descriptor descriptor);
		FRO_API Music(Music const& other);
		FRO_API Music(Music&& other) noexcept;

		FRO_API ~Music();

		FRO_API Music& operator=(Music const& other);
		FRO_API Music& operator=(Music&& other) noexcept;

		FRO_API FRO_NODISCARD Implementation& getImplementation() const;

		std::string_view getFilePath() const;

	private:
		Descriptor mDescriptor;
		std::unique_ptr<Implementation> mImplementation;
	};
}

#endif
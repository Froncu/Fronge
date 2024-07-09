#if not defined SOUND_EFFECT_HPP
#define SOUND_EFFECT_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Reference/Referencable.hpp"

namespace fro
{
	class SoundEffect final : public Referencable
	{
		friend class Audio;
		class Implementation;

	public:
		struct Descriptor final
		{
			std::string filePath;
		};

		FRO_API SoundEffect(Descriptor descriptor);
		FRO_API SoundEffect(SoundEffect const& other);
		FRO_API SoundEffect(SoundEffect&& other) noexcept;

		FRO_API ~SoundEffect();

		FRO_API SoundEffect& operator=(SoundEffect const& other);
		FRO_API SoundEffect& operator=(SoundEffect&& other) noexcept;

		FRO_API FRO_NODISCARD Implementation& getImplementation() const;

		FRO_API FRO_NODISCARD int getChannel() const;
		FRO_API FRO_NODISCARD std::string_view getFilePath() const;

	private:
		Descriptor mDescriptor;
		int mChannel{ -1 };

		std::unique_ptr<Implementation> mImplementation;
	};
}

#endif
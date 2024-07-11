#if not defined SOUND_EFFECT_HPP
#define SOUND_EFFECT_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Reference/Referencable.hpp"
#include "Idenitifer/IDGenerator.hpp"

namespace fro
{
	class SoundEffect final : public Referencable
	{
		friend class Audio;
		class Implementation;

	public:
		FRO_API SoundEffect(std::string_view const filePath);
		FRO_API SoundEffect(SoundEffect&& other) noexcept;

		FRO_API ~SoundEffect();

		FRO_API SoundEffect& operator=(SoundEffect&& other) noexcept;

		FRO_API FRO_NODISCARD Implementation& getImplementation() const;

		FRO_API FRO_NODISCARD std::size_t getID() const;
		FRO_API FRO_NODISCARD int getChannel() const;

	private:
		static IDGenerator sIDGenerator;

		SoundEffect(SoundEffect const&) = delete;

		SoundEffect& operator=(SoundEffect const&) = delete;

		ID mID{ sIDGenerator.get() };

		std::unique_ptr<Implementation> mImplementation;
		int mChannel{ -1 };
	};
}

#endif
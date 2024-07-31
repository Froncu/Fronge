#if not defined SOUND_EFFECT_IMPL_HPP
#define SOUND_EFFECT_IMPL_HPP

#include "froch.hpp"

#include "Resources/SoundEffect/SoundEffect.hpp"
#include "Utility/CustomUniquePointer.hpp"

struct Mix_Chunk;

namespace fro
{
	class SoundEffect::Implementation final
	{
	public:
		FRO_API Implementation(std::string_view const filePath);

		~Implementation() = default;

		FRO_API FRO_NODISCARD Mix_Chunk* getSDLSoundEffect() const;

	private:
		Implementation(Implementation const&) = delete;
		Implementation(Implementation&&) noexcept = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&&) noexcept = delete;

		CustomUniquePointer<Mix_Chunk> mSDLSoundEffect;
	};
}

#endif
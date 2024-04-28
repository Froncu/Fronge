#if not defined fro_PLAY_SOUND_EFFECT_H
#define fro_PLAY_SOUND_EFFECT_H

#include "Command.hpp"

namespace fro
{
	// HACK: bad, bad, bad; make command work with lambdas/callables & START MAKING THE GAME
	class PlaySoundEffect final : public Command
	{
	public:
		PlaySoundEffect(float const volume);

		~PlaySoundEffect() = default;

		virtual void operator()() override;

	private:
		PlaySoundEffect(PlaySoundEffect const&) = delete;
		PlaySoundEffect(PlaySoundEffect&&) noexcept = delete;

		PlaySoundEffect& operator=(PlaySoundEffect const&) = delete;
		PlaySoundEffect& operator=(PlaySoundEffect&&) noexcept = delete;

		float const m_Volume;
	};
	// END HACK
}

#endif
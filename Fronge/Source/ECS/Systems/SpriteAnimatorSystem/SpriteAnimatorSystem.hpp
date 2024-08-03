#if not defined SPRITE_ANIMATOR_SYSTEM_HPP
#define SPRITE_ANIMATOR_SYSTEM_HPP

#include "Core.hpp"
#include "ECS/Components/Sprite.hpp"
#include "ECS/Components/SpriteAnimator.hpp"
#include "ECS/Group.hpp"

namespace fro
{
	class SpriteAnimatorSystem final
	{
	public:
		SpriteAnimatorSystem() = default;
		SpriteAnimatorSystem(SpriteAnimatorSystem const&) = default;
		SpriteAnimatorSystem(SpriteAnimatorSystem&&) noexcept = default;

		~SpriteAnimatorSystem() = default;

		SpriteAnimatorSystem& operator=(SpriteAnimatorSystem const&) = default;
		SpriteAnimatorSystem& operator=(SpriteAnimatorSystem&&) noexcept = default;

		FRO_API void onUpdate(double const deltaSeconds);

	private:
		Group<Sprite, SpriteAnimator> const mGroup{};
	};
}

#endif
#ifndef SPRITE_ANIMATOR_SYSTEM_HPP
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
		FRO_API static void onUpdate(double const deltaSeconds);

	private:
		static Group<Sprite, SpriteAnimator> sGroup;

		SpriteAnimatorSystem() = delete;
		SpriteAnimatorSystem(SpriteAnimatorSystem const&) = delete;
		SpriteAnimatorSystem(SpriteAnimatorSystem&&) noexcept = delete;

		~SpriteAnimatorSystem() = delete;

		SpriteAnimatorSystem& operator=(SpriteAnimatorSystem const&) = delete;
		SpriteAnimatorSystem& operator=(SpriteAnimatorSystem&&) noexcept = delete;
	};
}

#endif
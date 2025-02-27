#ifndef SPRITE_ANIMATOR_HPP
#define SPRITE_ANIMATOR_HPP

#include "ecs/components/component.hpp"
#include "froch.hpp"
#include "maths/structs/rectangle.hpp"
#include "reference/reference.hpp"
#include "resources/sprite_animation.hpp"

namespace fro
{
	struct SpriteAnimator final : public Component
	{
	public:
		Reference<SpriteAnimation> animation{};
		std::size_t frameIndex{};
		double frameProgress{};
		bool play{ true };
	};
}

#endif
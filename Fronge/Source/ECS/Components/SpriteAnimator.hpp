#if not defined SPRITE_ANIMATOR_HPP
#define SPRITE_ANIMATOR_HPP

#include "froch.hpp"

#include "ECS/Components/Component.hpp"
#include "Maths/Structs/Rectangle.hpp"
#include "Reference/Reference.hpp"
#include "Resources/SpriteAnimation.hpp"

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
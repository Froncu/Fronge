#if not defined SPRITE_ANIMATOR_HPP
#define SPRITE_ANIMATOR_HPP

#include "froch.hpp"

#include "ECS/Components/Component.hpp"
#include "Maths/Structs/Rectangle.hpp"
#include "Reference/Reference.hpp"
#include "Resources/SpriteAnimationClip.hpp"

namespace fro
{
	struct SpriteAnimation final : public Component
	{
	public:
		Reference<SpriteAnimationClip> animationClip{};
		std::size_t currentFrameIndex{};
		double elapsedSeconds{};
		bool play{ true };
	};
}

#endif
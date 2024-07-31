#if not defined SPRITE_ANIMATION_CLIP_HPP
#define SPRITE_ANIMATION_CLIP_HPP

#include "froch.hpp"

#include "Maths/Structs/Rectangle.hpp"
#include "Reference/Referencable.hpp"

namespace fro
{
	struct SpriteAnimationClip final : public Referencable
	{
		std::vector<Rectangle<int>> sourceRectangles{};
		double frameTimeSeconds{ 1.0 / 5 };
		bool shouldLoop{ true };
	};
}

#endif
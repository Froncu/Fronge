#if not defined SPRITE_ANIMATION_HPP
#define SPRITE_ANIMATION_HPP

#include "froch.hpp"

#include "Maths/Structs/Rectangle.hpp"
#include "Reference/Referencable.hpp"

namespace fro
{
	struct SpriteAnimation final : public Referencable
	{
		std::vector<Rectangle<int>> sourceRectangles{};
		double framesPerSecond{ 5 };
		bool loops{ true };
	};
}

#endif
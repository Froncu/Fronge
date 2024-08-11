#if not defined SPRITE_HPP
#define SPRITE_HPP

#include "ECS/Components/Component.hpp"
#include "Maths/Structs/Rectangle.hpp"
#include "Reference/Reference.hpp"
#include "Resources/Texture/Texture.hpp"

namespace fro
{
	struct Sprite final : public Component
	{
	public:
		Reference<Texture const> texture{};
		Rectangle<int> sourceRectangle{};
		std::size_t layer{};
	};
}

#endif
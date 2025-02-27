#include "froch.hpp"

#include "SpriteSystem.hpp"

namespace fro
{
	void SpriteSystem::onRender(Renderer const& renderer)
	{
		for (auto&& [entity, transform, sprite] : sGroup)
			if (sprite->texture.valid())
				renderer.renderTexture(*sprite->texture, transform->world(), sprite->sourceRectangle);
	}

	Group<Transform, Sprite> SpriteSystem::sGroup
	{
		[](decltype(sGroup)::GroupTuple const& group1, decltype(sGroup)::GroupTuple const& group2)
		{
			return std::get<2>(group1)->layer < std::get<2>(group2)->layer;
		}
	};
}
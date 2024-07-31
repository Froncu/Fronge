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

	Group<Transform, Sprite> SpriteSystem::sGroup{};
}
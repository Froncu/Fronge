#ifndef SPRITE_SYSTEM_HPP
#define SPRITE_SYSTEM_HPP

#include "Core.hpp"
#include "ECS/Components/Sprite.hpp"
#include "ECS/Components/Transform/Transform.hpp"
#include "ECS/Group.hpp"
#include "Renderer/Renderer.hpp"

namespace fro
{
	class SpriteSystem final
	{
	public:
		FRO_API static void onRender(Renderer const& renderer);

	private:
		static Group<Transform, Sprite> sGroup;

		SpriteSystem() = delete;
		SpriteSystem(SpriteSystem const&) = delete;
		SpriteSystem(SpriteSystem&&) noexcept = delete;

		~SpriteSystem() = delete;

		SpriteSystem& operator=(SpriteSystem const&) = delete;
		SpriteSystem& operator=(SpriteSystem&&) noexcept = delete;
	};
}

#endif
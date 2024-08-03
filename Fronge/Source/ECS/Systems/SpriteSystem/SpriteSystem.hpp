#if not defined SPRITE_SYSTEM_HPP
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
		SpriteSystem() = default;
		SpriteSystem(SpriteSystem const&) = default;
		SpriteSystem(SpriteSystem&&) noexcept = default;

		~SpriteSystem() = default;

		SpriteSystem& operator=(SpriteSystem const&) = default;
		SpriteSystem& operator=(SpriteSystem&&) noexcept = default;

		FRO_API void onRender(Renderer const& renderer);

	private:
		Group<Transform, Sprite> const mGroup{};
	};
}

#endif
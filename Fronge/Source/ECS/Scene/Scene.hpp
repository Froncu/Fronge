#if not defined SCENE_HPP
#define SCENE_HPP

#include "froch.hpp"

#include "ECS/Entity/Entity.hpp"

namespace fro
{
	class Scene final
	{
	public:
		Scene() = default;
		Scene(Scene const&) = default;
		Scene(Scene&&) noexcept = default;

		~Scene() = default;

		Scene& operator=(Scene const&) = default;
		Scene& operator=(Scene&&) noexcept = default;

		FRO_API void doomEntities();

		FRO_API std::optional<Entity> removeEntity(Entity const& entity);

		template<typename... ArgumentTypes>
			requires std::constructible_from<Entity, ArgumentTypes...>
		Entity& addEntity(ArgumentTypes&&... arguments)
		{
			return mEntities.emplace_back(std::forward<ArgumentTypes>(arguments)...);
		}

	private:
		std::vector<Entity> mEntities{};
	};
}

#endif
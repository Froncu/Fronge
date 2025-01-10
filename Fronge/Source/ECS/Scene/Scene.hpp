#ifndef SCENE_HPP
#define SCENE_HPP

#include "froch.hpp"

#include "ECS/Entity/Entity.hpp"
#include "Reference/Referencable.hpp"

namespace fro
{
	class Scene final : public Referencable
	{	
	public:
		Scene() = default;
		Scene(Scene const&) = default;
		Scene(Scene&&) noexcept = default;

		virtual ~Scene() override = default;

		Scene& operator=(Scene const&) = default;
		Scene& operator=(Scene&&) noexcept = default;

		void doomAndAdd();

		FRO_API FRO_NODISCARD std::vector<Entity> const& getEntities() const;

		FRO_API std::optional<Entity> removeEntity(Entity const& entity);

		template<typename... ArgumentTypes>
			requires std::constructible_from<Entity, ArgumentTypes...>
		Entity& addEntity(ArgumentTypes&&... arguments)
		{
			return mEntitiesToAdd.emplace_back(std::forward<ArgumentTypes>(arguments)...);
		}

	private:
		std::vector<Entity> mEntities{};
		std::vector<Entity> mEntitiesToAdd{};
	};
}

#endif
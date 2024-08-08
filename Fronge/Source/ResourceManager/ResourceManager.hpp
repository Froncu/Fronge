#if not defined RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Logger/Logger.hpp"

namespace fro
{
	class ResourceManager final
	{
	public:
		template<typename ResourceType, typename NameType, typename... ArgumentTypes>
			requires std::constructible_from<ResourceType, ArgumentTypes...> and std::constructible_from<std::string, NameType>
		static ResourceType& store(NameType&& name, ArgumentTypes&&... arguments)
		{
			auto& resourceMap{ getResourceMap<ResourceType>() };
			auto&& [element, didInsert] { resourceMap.try_emplace(
				std::forward<NameType>(name), 
				std::forward<ArgumentTypes>(arguments)...) };

			if (didInsert)
				Logger::info("stored a {} named \"{}\" in the ResourceManager!",
					typeid(ResourceType).name(), element->first);
			else
				Logger::warn("failed to store a {} named \"{}\" in the ResourceManager (name is already taken for this type)",
					typeid(ResourceType).name(), element->first);

			return element->second;
		}

		template<std::movable ResourceType>
		static std::optional<ResourceType> free(std::string_view const name)
		{
			auto& resourceMap{ getResourceMap<ResourceType>() };

			// HACK: this and the following two methods construct a temporary std::string to compare,
			// this should be fixed with a custom hasher and comparer
			auto node{ resourceMap.extract(name.data()) };
			if (node.empty())
			{
				Logger::warn("failed to free a {} named \"{}\" from the ResourceManager (it's not stored)",
					typeid(ResourceType).name(), name);

				return std::nullopt;
			}

			Logger::info("freed a {} named \"{}\" from the ResourceManager!",
				typeid(ResourceType).name(), name);

			return std::move(node.mapped());
		}

		template<typename ResourceType>
		static bool free(std::string_view const name)
		{
			bool const didFree{ getResourceMap<ResourceType>().erase(name.data()) };
			if (didFree)
				Logger::info("freed a {} named \"{}\" from the ResourceManager!",
					typeid(ResourceType).name(), name);
			else
				Logger::warn("failed to free a {} named \"{}\" from the ResourceManager (it's not stored)",
					typeid(ResourceType).name(), name);;
		}

		template<typename ResourceType>
		FRO_NODISCARD static ResourceType* find(std::string_view const name)
		{
			auto& resourceMap{ getResourceMap<ResourceType>() };

			auto const element{ resourceMap.find(name.data()) };
			if (element == resourceMap.end())
				return nullptr;

			return &element->second;
		}

	private:
		template<typename ResourceType>
		static auto& getResourceMap()
		{
			static std::unordered_map<std::string, ResourceType> resourceMap{};
			return resourceMap;
		}

		ResourceManager() = delete;
		ResourceManager(ResourceManager const&) = delete;
		ResourceManager(ResourceManager&&) noexcept = delete;

		~ResourceManager() = delete;

		ResourceManager& operator=(ResourceManager const&) = delete;
		ResourceManager& operator=(ResourceManager&&) noexcept = delete;
	};
}

#endif
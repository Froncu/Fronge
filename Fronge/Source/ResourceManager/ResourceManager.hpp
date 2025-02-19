#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "Core.hpp"
#include "froch.hpp"
#include "Logger/Logger.hpp"

namespace fro
{
   class ResourceManager final
   {
      public:
         ResourceManager() = default;
         ResourceManager(ResourceManager const&) = delete;
         ResourceManager(ResourceManager&&) noexcept = delete;

         ~ResourceManager() = default;

         ResourceManager& operator=(ResourceManager const&) = delete;
         ResourceManager& operator=(ResourceManager&&) noexcept = delete;

         template<typename ResourceType, typename NameType, typename... ArgumentTypes> requires
            std::constructible_from<ResourceType, ArgumentTypes...> and std::constructible_from<std::string, NameType>
         ResourceType& store(NameType&& name, ArgumentTypes&&... arguments)
         {
            auto&& [element, didInsert]{
               getResourceMap<ResourceType>().try_emplace(
                  std::forward<NameType>(name),
                  std::forward<ArgumentTypes>(arguments)...)
            };

            if (didInsert)
               Logger::info("stored a {} named \"{}\" in the ResourceManager!",
                  typeid(ResourceType).name(), element->first);
            else
               Logger::warn("failed to store a {} named \"{}\" in the ResourceManager (name is already taken for this type)",
                  typeid(ResourceType).name(), element->first);

            return element->second;
         }

         template<std::movable ResourceType>
         std::optional<ResourceType> free(std::string_view const name)
         {
            // HACK: this and the following two methods construct a temporary std::string to compare,
            // this should be fixed with a custom hasher and comparer
            auto&& node{ getResourceMap<ResourceType>().extract(name.data()) };
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
         bool free(std::string_view const name)
         {
            bool const didFree{ static_cast<bool>(getResourceMap<ResourceType>().erase(name.data())) };
            if (didFree)
               Logger::info("freed a {} named \"{}\" from the ResourceManager!",
                  typeid(ResourceType).name(), name);
            else
               Logger::warn("failed to free a {} named \"{}\" from the ResourceManager (it's not stored)",
                  typeid(ResourceType).name(), name);

            return didFree;
         }

         template<std::copyable ResourceType>
         FRO_NODISCARD ResourceType* find(std::string_view const name)
         {
            auto& resourceMap{ getResourceMap<ResourceType>() };

            auto const element{ resourceMap.find(name.data()) };
            if (element == resourceMap.end())
               return nullptr;

            return &element->second;
         }

      private:
         std::unordered_map<std::type_index, std::any> mResourceMaps{};

         // TODO: the resources in this class will never be copied,
         // however std::any requires the std::unordered_maps to be copyable
         // thus requiring the resources to be copyable, maybe there
         // is a workaround/better approach
         template<std::copyable ResourceType>
         auto&& getResourceMap()
         {
            using MapType = std::unordered_map<std::string, ResourceType>;

            std::type_index const typeIndex{ typeid(ResourceType) };

            if (auto typeMap{ mResourceMaps.find(typeIndex) }; typeMap not_eq mResourceMaps.end())
               return std::any_cast<MapType&>(typeMap->second);

            return std::any_cast<MapType&>(mResourceMaps.emplace(typeIndex, MapType{}).first->second);
         }
   };
}

#endif
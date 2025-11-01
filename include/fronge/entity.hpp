#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "fronge/id.hpp"
#include "fronge/scene.hpp"

namespace fro
{
   class Scene;

   class Entity final : public Referenceable
   {
      friend Scene;

      public:
         Entity(Entity const&) = delete;
         Entity(Entity&&) = delete;

         FRO_API virtual ~Entity() override;

         Entity& operator=(Entity const&) = delete;
         Entity& operator=(Entity&&) = delete;

         template <Componentable Component, typename... Arguments>
            requires std::constructible_from<Component, Arguments...>
         Component& add_component(Arguments&&... arguments)
         {
            return std::get<Scene::ComponentSparseSet<Component>>(scene_->component_sparse_sets_).enqueue_add(
               static_cast<ID::InternalValue>(id()), std::forward<Arguments>(arguments)...);
         }

         template <Componentable Component>
         void remove_component()
         {
            std::get<Scene::ComponentSparseSet<Component>>(scene_->component_sparse_sets_).enqueue_remove(
               static_cast<ID::InternalValue>(id()));
         }

         template <Componentable Component>
         [[nodiscard]] Component* find_component()
         {
            return std::get<Scene::ComponentSparseSet<Component>>(scene_->component_sparse_sets_).find(
               static_cast<ID::InternalValue>(id()));
         }

         template <Componentable Component>
         [[nodiscard]] Component const* find_component() const
         {
            return std::get<Scene::ComponentSparseSet<Component>>(scene_->component_sparse_sets_).find(
               static_cast<ID::InternalValue>(id()));
         }

         template <Componentable Component>
         [[nodiscard]] bool contains_component() const
         {
            return std::get<Scene::ComponentSparseSet<Component>>(scene_->component_sparse_sets_).contains(
               static_cast<ID::InternalValue>(id()));
         }

         ID const& id() const
         {
            return id_;
         }

      private:
         static IDGenerator id_generator_;

         explicit Entity(Scene const& scene);

         ID id_{ id_generator_.generate() };
         Reference<Scene const> scene_;
   };
}

#endif
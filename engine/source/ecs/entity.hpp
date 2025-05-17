#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "identifier/id.hpp"
#include "scene.hpp"

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

         template <SparseSetStorable Component, typename... Arguments>
            requires std::constructible_from<Component, Arguments...>
         Component& add_component(Arguments&&... arguments)
         {
            return scene_->component_sparse_set<Component>().enqueue_add(static_cast<ID::InternalValue>(id()),
               std::forward<Arguments>(arguments)...);
         }

         template <SparseSetStorable Component>
         void remove_component()
         {
            scene_->component_sparse_set<Component>().enqueue_remove(static_cast<ID::InternalValue>(id()));
         }

         void remove_components()
         {
            for (std::unique_ptr<Scene::BaseComponentSparseSet> const& component_sparse_set :
               std::views::values(scene_->component_sparse_sets_))
               component_sparse_set->enqueue_remove(static_cast<ID::InternalValue>(id()));
         }

         template <SparseSetStorable Component>
         [[nodiscard]] Component* find_component()
         {
            return scene_->component_sparse_set<Component>().find(static_cast<ID::InternalValue>(id()));
         }

         template <SparseSetStorable Component>
         [[nodiscard]] Component const* find_component() const
         {
            return scene_->component_sparse_set<Component>().find(static_cast<ID::InternalValue>(id()));
         }

         template<SparseSetStorable Component>
         [[nodiscard]] bool contains_component() const
         {
            return scene_->component_sparse_set<Component>().contains(static_cast<ID::InternalValue>(id()));
         }

         ID const& id() const
         {
            return id_;
         }

      private:
         static IDGenerator id_generator_;

         explicit Entity(Scene& scene);

         ID id_{ id_generator_.generate() };
         Reference<Scene> scene_;
   };
}

#endif
#ifndef GROUP_HPP
#define GROUP_HPP

#include "scene.hpp"
#include "froch.hpp"
#include "reference/reference.hpp"
#include "utility/template_parameter_pack.hpp"
#include "utility/unique_parameter_pack.hpp"

namespace fro
{
   class Entity;

   template <typename OwnedComponents, typename ObservedComponents>
   class Group;

   template <typename... OwnedComponents, typename... ObservedComponents>
      requires IS_UNIQUE<OwnedComponents..., ObservedComponents...>
   class Group<TemplateParameterPack<OwnedComponents...>, TemplateParameterPack<ObservedComponents...>> final
   {
      friend Scene;

      public:
         explicit Group(Scene& scene)
            : scene_{ scene }
         {
         }

         Group(Group const&) = delete;
         Group(Group&&) noexcept = delete;

         ~Group() = default;

         Group& operator=(Group const&) = delete;
         Group& operator=(Group&&) noexcept = delete;

         [[nodiscard]] auto begin()
         {
            return view_.begin();
         }

         [[nodiscard]] auto end()
         {
            return view_.end();
         }

         void update()
         {
            view_ = construct_view();
         }

      private:
         [[nodiscard]] auto construct_view()
         {
            std::tuple<Scene::ComponentSparseSet<OwnedComponents> &...> owned_component_sparse_sets{
               scene_->component_sparse_set<OwnedComponents>()...
            };

            std::tuple<Scene::ComponentSparseSet<ObservedComponents> &...> observed_component_sparse_sets{
               scene_->component_sparse_set<ObservedComponents>()...
            };

            entities_.clear();
            (std::get<std::vector<ObservedComponents*>>(observed_components_).clear(), ...);

            for (std::unique_ptr<Entity> const& entity : scene_->entities_)
            {
               ID::InternalValue const entity_id{ entity->id() };

               bool const can_group_owned{
                  (std::get<Scene::ComponentSparseSet<OwnedComponents>&>(owned_component_sparse_sets).contains(entity_id)
                     and ...)
               };

               bool const can_group_observed{
                  (std::get<Scene::ComponentSparseSet<ObservedComponents>&>(observed_component_sparse_sets).contains(entity_id)
                     and ...)
               };

               if (not can_group_owned or not can_group_observed)
                  continue;

               (std::get<Scene::ComponentSparseSet<OwnedComponents>&>(owned_component_sparse_sets)
                  .move(entity_id, entities_.size()), ...);

               (std::get<std::vector<ObservedComponents*>>(observed_components_)
                  .push_back(std::get<Scene::ComponentSparseSet<ObservedComponents>&>(observed_component_sparse_sets)
                     .find(entity_id)), ...);

               entities_.push_back(entity.get());
            }

            return std::views::zip(
               entities_ |
               std::views::transform(std::function<Entity&(Entity*)>{
                  [](Entity* const entity) -> Entity&
                  {
                     return *entity;
                  }
               }),

               std::get<Scene::ComponentSparseSet<OwnedComponents>&>(owned_component_sparse_sets).components()...,

               std::get<std::vector<ObservedComponents*>>(observed_components_) |
               std::views::transform(std::function<ObservedComponents&(ObservedComponents*)>{
                  [](ObservedComponents* const observed_component) -> ObservedComponents&
                  {
                     return *observed_component;
                  }
               })...);
         }

         Reference<Scene> scene_;
         std::vector<Entity*> entities_{};
         std::tuple<std::vector<ObservedComponents*...>> observed_components_{};

         // NOTE: std::invoke_result_t does not work on MSVC here because Group
         // is still considered undefined, whereas GCC works fine
         std::ranges::zip_view<
            std::ranges::transform_view<
               std::ranges::ref_view<std::vector<Entity*>>,
               std::function<Entity&(Entity*)>>,
            std::span<OwnedComponents>...,
            std::ranges::transform_view<
               std::ranges::ref_view<std::vector<ObservedComponents*>>,
               std::function<ObservedComponents&(ObservedComponents*)>>...> view_{ construct_view() };
   };
}

#endif
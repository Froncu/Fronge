#ifndef GROUP_HPP
#define GROUP_HPP

#include "scene.hpp"
#include "components/components.hpp"
#include "froch.hpp"
#include "reference/reference.hpp"
#include "utility/template_parameter_pack.hpp"
#include "utility/unique_parameter_pack.hpp"

namespace fro
{
   class Entity;

   template <typename OwnedComponents, typename ObservedComponents>
   class Group;

   template <Componentable... OwnedComponents, Componentable... ObservedComponents>
      requires IS_UNIQUE<OwnedComponents..., ObservedComponents...>
   class Group<TemplateParameterPack<OwnedComponents...>, TemplateParameterPack<ObservedComponents...>> final
      : public Scene::BaseGroup
   {
      friend Scene;

      public:
         Group(Group const&) = delete;
         Group(Group&&) noexcept = delete;

         virtual ~Group() override = default;

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

      private:
         explicit Group(Scene& scene)
            : scene_{ scene }
         {
         }

         virtual void update() override
         {
            view_ = construct_view();
         }

         [[nodiscard]] auto construct_view()
         {
            std::tuple<Scene::ComponentSparseSet<OwnedComponents>&...> const owned_component_sparse_sets{
               std::get<Scene::ComponentSparseSet<OwnedComponents>>(scene_->component_sparse_sets_)...
            };

            std::tuple<Scene::ComponentSparseSet<ObservedComponents>&...> const observed_component_sparse_sets{
               std::get<Scene::ComponentSparseSet<ObservedComponents>>(scene_->component_sparse_sets_)...
            };

            entities_.clear();
            (std::get<std::vector<ObservedComponents*>>(observed_components_).clear(), ...);

            for (std::unique_ptr<Entity> const& entity : scene_->entities_)
            {
               ID::InternalValue const entity_id{ entity->id() };

               bool const can_group_owned_components{
                  (std::get<Scene::ComponentSparseSet<OwnedComponents>&>(owned_component_sparse_sets).contains(entity_id)
                     and ...)
               };

               std::tuple<ObservedComponents*...> const observed_components{
                  std::get<Scene::ComponentSparseSet<ObservedComponents>&>(observed_component_sparse_sets)
                  .find(entity_id)...
               };

               if (not can_group_owned_components or (not std::get<ObservedComponents*>(observed_components) or ...))
                  continue;

               (std::get<Scene::ComponentSparseSet<OwnedComponents>&>(owned_component_sparse_sets)
                  .move(entity_id, entities_.size()), ...);

               (std::get<std::vector<ObservedComponents*>>(observed_components_)
                  .push_back(std::get<ObservedComponents*>(observed_components)), ...);

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
#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "core.hpp"
#include "ecs/components/component.hpp"
#include "ecs/entity/entity.hpp"
#include "maths/structs/transform_matrix_3x3/transform_matrix_3x3.hpp"
#include "reference/reference.hpp"

namespace fro
{
   class Transform final : public Component
   {
      public:
         Transform() = default;
         Transform(Transform const&) = default;
         Transform(Transform&&) noexcept = default;

         FRO_API virtual ~Transform() override;

         Transform& operator=(Transform const&) = default;
         Transform& operator=(Transform&&) noexcept = default;

         FRO_API FRO_NODISCARD Reference<Entity> getParentingEntity() const;

         FRO_API bool setParent(Transform* const parent, bool keepWorldTransform = true);

         FRO_API FRO_NODISCARD bool isParenting(Transform const& transform) const;
         FRO_API FRO_NODISCARD std::vector<Reference<Transform>> const& getChildren() const;

         FRO_API void setLocalTranslation(Vector2<double> const translation);
         FRO_API void setLocalRotation(double const rotation);
         FRO_API void setLocalScale(Vector2<double> const scale);

         FRO_API void localTranslate(Vector2<double> const translation);
         FRO_API void localRotate(double const rotation);
         FRO_API void localScale(Vector2<double> const scale);

         FRO_API void setWorldTranslation(Vector2<double> const translation);
         FRO_API void setWorldRotation(double const rotation);
         FRO_API void setWorldScale(Vector2<double> const scale);

         FRO_API void worldTranslate(Vector2<double> const translation);
         FRO_API void worldRotate(double const rotation);
         FRO_API void worldScale(Vector2<double> const scale);

         FRO_API FRO_NODISCARD TransformMatrix3x3 const& local() const;
         FRO_API FRO_NODISCARD TransformMatrix3x3 const& world() const;

      private:
         void setWorldTransformDirty();
         FRO_NODISCARD TransformMatrix3x3& getWorldTransform() const;
         void calculateWorldTransform() const;
         void calculateLocalTransform();

         EventListener<Entity, Component, std::type_index const> mOnComponentAttachEvent
         {
            [smartThis = Reference{ this }](Entity& entity, Component const& component, std::type_index const&)
            {
               if (&*smartThis not_eq &component)
                  return false;

               smartThis->mParentingEntity = entity;
               return true;
            },
            EntityManager::getComponentAttachEvent()
         };

         EventListener<Entity, Component, std::type_index const> mOnComponentDetachEvent
         {
            [smartThis = Reference{ this }](Entity const&, Component const& component, std::type_index const&)
            {
               if (&*smartThis not_eq &component)
                  return false;

               smartThis->mParentingEntity.reset();
               return true;
            },
            EntityManager::getComponentDetachEvent()
         };

         TransformMatrix3x3 mLocalTransform{};
         mutable TransformMatrix3x3 mWorldTransform{};

         std::vector<Reference<Transform>> mChildren{};
         Reference<Transform> mParent{};

         Reference<Entity> mParentingEntity{};

         mutable bool mIsWorldTransformDirty{};
   };
}

#endif
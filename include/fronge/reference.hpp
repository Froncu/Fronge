#ifndef REFERENCE_HPP
#define REFERENCE_HPP

#include "fronge/base_reference.hpp"

namespace fro
{
   // TODO: there should be constraints/static asserts on Type to be both derived from
   // Referenceable and not from BaseReference. This is not implemented because such
   // constraints limits the Reference when used as a data member; class A cannot hold
   // a data member of type Reference<A> even though A because A is not complete when
   // the constraints are checked.

   template <typename Type>
   concept NonReference =
      not std::is_reference_v<Type>;

   template <typename From, typename To>
   concept ConstantPromotable =
      not std::is_const_v<From> or
      std::is_const_v<To>;

   template <typename From, typename To>
   concept ReferenceConstructable =
      NonReference<From> and
      ConstantPromotable<From, To> and
      std::derived_from<From, To>;

   template <typename... Ts>
   concept Polymorphic =
      (std::is_polymorphic_v<Ts> and ...);

   template <typename T1, typename T2>
   concept EitherDerived =
      std::derived_from<T1, T2> or
      std::derived_from<T2, T1>;

   template <NonReference T>
   class Reference final : public BaseReference
   {
      public:
         Reference() = default;

         template <ReferenceConstructable<T> Other>
         explicit Reference(Other* const referenceable)
            : BaseReference(referenceable)
         {
         }

         template <ReferenceConstructable<T> Other>
         explicit Reference(Other& referenceable)
            : BaseReference(referenceable)
         {
         }

         explicit Reference(T* const referenceable)
            : BaseReference(referenceable)
         {
         }

         explicit Reference(T& referenceable)
            : BaseReference(referenceable)
         {
         }

         template <ReferenceConstructable<T> Other>
         explicit Reference(Reference<Other> const* const referenceable)
            : BaseReference(referenceable)
         {
         }

         template <ReferenceConstructable<T> Other>
         explicit Reference(Reference<Other> const& referenceable)
            : BaseReference(referenceable)
         {
         }

         Reference(Reference const& other)
            : BaseReference(other)
         {
         }

         Reference(Reference&& other) noexcept
            : BaseReference(std::move(other))
         {
         }

         virtual ~Reference() override = default;

         Reference& operator=(Reference const& other)
         {
            BaseReference::operator=(other);
            return *this;
         }

         Reference& operator=(Reference&& other) noexcept
         {
            BaseReference::operator=(std::move(other));
            return *this;
         }

         [[nodiscard]] T* operator->() const
         {
            return &get();
         }

         [[nodiscard]] T& operator*() const
         {
            return get();
         }

         template <NonReference Other>
            requires Polymorphic<T, Other> and ConstantPromotable<T, Other>
         [[nodiscard]] Reference<Other> cast_dynamically() const
         {
            return dynamic_cast<Other* const>(referenceable_);
         }

         template <NonReference Other>
            requires EitherDerived<T, Other> and ConstantPromotable<T, Other>
         [[nodiscard]] Reference<Other> cast_statically() const
         {
            return static_cast<Other* const>(referenceable_);
         }

         [[nodiscard]] T& get() const
         {
            return *static_cast<T* const>(referenceable_);
         }
   };
}

#endif
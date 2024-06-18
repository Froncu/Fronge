#if not defined fro_REFERENCE_H
#define fro_REFERENCE_H

#include "BaseReference.h"
#include "Referencable.hpp"

#include <utility>

namespace fro
{
	template<typename ...Types>
	concept SameReferencableType =
		DerivedReferencableType<Types...> or
		(not DerivedReferencableType<Types> and ...);

	template<typename Type1, typename Type2>
	concept EitherDerivedType =
		std::derived_from<Type1, Type2> or
		std::derived_from<Type2, Type1>;

	template<typename ...Types>
	concept PolymorphicType =
		(std::is_polymorphic_v<Types> and ...);

	template<typename Type>
	class Reference final : public BaseReference
	{
		static_assert(not SpecializedReferencableType<Type>,
			"the referenced type cannot be a specialization of Referencable");

		static_assert(not SpecializedType<Type, Reference>,
			"the referenced type cannot be a specialization of Reference");

		static_assert(not std::same_as<Type, BaseReferencable>,
			"the referenced type cannot be BaseReferencable");

		static_assert(not std::is_reference_v<Type>,
			"the referenced type cannot be a reference");

	public:
		Reference() = default;

		template<std::derived_from<Type> OtherType>
			requires DerivedReferencableType<Type, OtherType>
		Reference(OtherType* const pReferencable)
			: BaseReference(pReferencable)
		{
		}

		template<std::derived_from<Type> OtherType>
			requires DerivedReferencableType<Type, OtherType>
		Reference(OtherType& referencable)
			: BaseReference(referencable)
		{
		}

		Reference(Type* const pReferencable)
			requires DerivedReferencableType<Type>
		: BaseReference(pReferencable)
		{
		}

		Reference(Type& referencable)
			requires DerivedReferencableType<Type>
		: BaseReference(referencable)
		{
		}

		template<std::derived_from<Type> OtherType>
		Reference(Referencable<OtherType>* const pReferencable)
			: BaseReference(pReferencable)
		{
		}

		template<std::derived_from<Type> OtherType>
		Reference(Referencable<OtherType>& referencable)
			: BaseReference(referencable)
		{
		}

		Reference(Referencable<Type>* const pReferencable)
			: BaseReference(pReferencable)
		{
		}
		
		Reference(Referencable<Type>& referencable)
			: BaseReference(referencable)
		{
		}

		template<std::derived_from<Type> OtherType>
			requires SameReferencableType<Type, OtherType>
		Reference(Reference<OtherType> const* const pReferencable)
			: BaseReference(pReferencable)
		{
		}

		template<std::derived_from<Type> OtherType>
			requires SameReferencableType<Type, OtherType>
		Reference(Reference<OtherType> const& referencable)
			: BaseReference(referencable)
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

		template<typename OtherType>
			requires PolymorphicType<Type, OtherType> and SameReferencableType<Type, OtherType>
		Reference<OtherType> dynamicCast() const
		{
			return dynamic_cast<OtherType* const>(getReferencable());
		}

		template<EitherDerivedType<Type> OtherType>
			requires SameReferencableType<Type, OtherType>
		Reference<OtherType> staticCast() const
		{
			return static_cast<OtherType* const>(getReferencable());
		}

		Type& get() const
		{
			BaseReferencable* const pReferencable{ getReferencable() };

			if constexpr (DerivedReferencableType<Type>)
				return *static_cast<Type* const>(pReferencable);
			else
				return static_cast<Referencable<Type>*const>(pReferencable)->get();
		}
	};
}

#endif
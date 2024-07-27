#if not defined REFERENCE_H
#define REFERENCE_H

#include "froch.hpp"

#include "BaseReference.hpp"
#include "Core.hpp"
#include "Referencable.hpp"

namespace fro
{
	// TODO: there should be a constraint/static assert on Type to be derived from Referencable.
	// This is not implemented because such a constraint limits the Reference when used as a
	// data member; class A cannot hold a data member of type Reference<A> even though A
	// derives from Referencable. This is because A is not complete when the constraint
	// is checked.

	template<typename ...Types>
	concept PolymorphicType =
		(std::is_polymorphic_v<Types> and ...);

	template<typename Type1, typename Type2>
	concept EitherDerivedType =
		std::derived_from<Type1, Type2> or
		std::derived_from<Type2, Type1>;
	
	template<typename FromType, typename ToType>
	concept ConstantPromotableType =
		(std::is_const_v<FromType> and std::is_const_v<ToType>) or
		(not std::is_const_v<FromType> and std::is_const_v<ToType>) or
		(not std::is_const_v<FromType> and not std::is_const_v<ToType>);

	// TODO: shitty name, *very* shitty
	template<typename FromType, typename ToType>
	concept ReferenceConstructorType =
		ReferencableType<FromType> and
		ConstantPromotableType<FromType, ToType> and
		std::derived_from<FromType, ToType>;

	template<typename Type>
	class Reference final : public BaseReference
	{
		static_assert(ReferencableType<Type>,
			"the referenced type cannot be a reference or a specialization of Reference");

	public:
		Reference() = default;

		template<ReferenceConstructorType<Type> OtherType>
		Reference(OtherType* const pReferencable)
			: BaseReference(pReferencable)
		{
		}

		template<ReferenceConstructorType<Type> OtherType>
		Reference(OtherType& referencable)
			: BaseReference(referencable)
		{
		}

		Reference(Type* const pReferencable)
			: BaseReference(pReferencable)
		{
		}

		Reference(Type& referencable)
			: BaseReference(referencable)
		{
		}

		template<ReferenceConstructorType<Type> OtherType>
		Reference(Reference<OtherType> const* const pReferencable)
			: BaseReference(pReferencable)
		{
		}

		template<ReferenceConstructorType<Type> OtherType>
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

		Type* operator->() const
		{
			return &get();
		}

		Type& operator*() const
		{
			return get();
		}

		template<ReferencableType OtherType>
			requires PolymorphicType<Type, OtherType> and ConstantPromotableType<Type, OtherType>
		Reference<OtherType> dynamicCast() const
		{
			return dynamic_cast<OtherType* const>(mReferencable);
		}

		template<ReferencableType OtherType>
			requires EitherDerivedType<Type, OtherType> and ConstantPromotableType<Type, OtherType>
		Reference<OtherType> staticCast() const
		{
			return static_cast<OtherType* const>(mReferencable);
		}

		Type& get() const
		{
			return *static_cast<Type* const>(mReferencable);
		}
	};
}

template<typename Type>
struct std::hash<fro::Reference<Type>>
{
	std::size_t operator()(fro::Reference<Type> const& reference) const noexcept
	{
		return std::hash<fro::BaseReference>{}(static_cast<fro::BaseReference const&>(reference));
	}
};

#endif
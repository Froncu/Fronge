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

	template<typename Type>
	class Reference final : public BaseReference
	{
		static_assert(ReferencableType<Type>,
			"the referenced type cannot be a reference or a specialization of Reference");

	public:
		Reference() = default;

		template<std::derived_from<Type> OtherType>
		Reference(OtherType* const pReferencable)
			: BaseReference(pReferencable)
		{
		}

		template<std::derived_from<Type> OtherType>
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

		template<std::derived_from<Type> OtherType>
		Reference(Reference<OtherType> const* const pReferencable)
			: BaseReference(pReferencable)
		{
		}

		template<std::derived_from<Type> OtherType>
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

		template<typename OtherType>
			requires PolymorphicType<Type, OtherType>
		Reference<OtherType> dynamicCast() const
		{
			return dynamic_cast<OtherType* const>(mReferencable);
		}

		template<typename OtherType>
			requires EitherDerivedType<Type, OtherType>
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

#endif
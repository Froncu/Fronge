#if not defined BASE_REFERENCE_HPP
#define BASE_REFERENCE_HPP

#include "froch.hpp"

#include "Core.hpp"

namespace fro
{
	class BaseReference
	{
		friend class Referencable;

		template<typename>
		friend class Reference;

	public:
		FRO_API virtual ~BaseReference();

		FRO_API bool operator==(Referencable const* const pReferencable) const;
		FRO_API bool operator==(Referencable const& referencable) const;
		FRO_API bool operator==(BaseReference const& other) const;
		FRO_API std::strong_ordering operator<=>(Referencable const* const pReferencable) const;
		FRO_API std::strong_ordering operator<=>(Referencable const& referencable) const;
		FRO_API std::strong_ordering operator<=>(BaseReference const& other) const;

		FRO_API void reset();
		FRO_API bool valid() const;

	private:
		BaseReference() = default;
		FRO_API BaseReference(Referencable* const pReferencable);
		FRO_API BaseReference(Referencable& referencable);
		FRO_API BaseReference(BaseReference const& other);
		FRO_API BaseReference(BaseReference&& other) noexcept;

		FRO_API BaseReference& operator=(BaseReference const& other);
		FRO_API BaseReference& operator=(BaseReference&& other) noexcept;

		Referencable* mReferencable{};
	};

	template<typename Type>
	concept ReferencableType =
		not std::derived_from<Type, BaseReference> and not std::is_reference_v<Type>;
}

#endif
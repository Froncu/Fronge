#if not defined fro_BASE_REFERENCE_H
#define fro_BASE_REFERENCE_H

#include <compare>

namespace fro
{
	class BaseReferencable;

	class BaseReference
	{
		friend BaseReferencable;

	public:
		virtual ~BaseReference();

		bool operator==(BaseReferencable const* const pReferencable) const;
		bool operator==(BaseReferencable const& referencable) const;
		bool operator==(BaseReference const& other) const;
		std::strong_ordering operator<=>(BaseReferencable const* const pReferencable) const;
		std::strong_ordering operator<=>(BaseReferencable const& referencable) const;
		std::strong_ordering operator<=>(BaseReference const& other) const;

		void reset();

		bool valid() const;

	protected:
		BaseReference() = default;
		BaseReference(BaseReferencable* const pReferencable);
		BaseReference(BaseReferencable& referencable);
		BaseReference(BaseReference const& other);
		BaseReference(BaseReference&& other) noexcept;

		BaseReference& operator=(BaseReference const& other);
		BaseReference& operator=(BaseReference&& other) noexcept;

	protected:
		BaseReferencable* getReferencable() const;

	private:
		BaseReferencable* m_pReferencable{};
	};
}

#endif
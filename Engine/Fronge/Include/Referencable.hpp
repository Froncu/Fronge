#if not defined fro_REFERENCABLE_H
#define fro_REFERENCABLE_H

#include <set>

namespace fro
{
	template<typename>
	class Reference;

	template<typename Type>
	class Referencable
	{
		friend Reference<Type>;
		
	public:
		Referencable() = default;
		Referencable(Referencable const&) = delete;
		Referencable(Referencable&& other) noexcept
			: m_spReferences{ std::move(other.m_spReferences) }
		{
			for (Reference<Type>* const pReference : m_spReferences)
				pReference->m_pReferencable = this;
		}

		virtual ~Referencable()
		{
			for (Reference<Type>* const pReference : m_spReferences)
				pReference->m_pReferencable = nullptr;
		}

		Referencable& operator=(Referencable const&) = delete;
		Referencable& operator=(Referencable&& other) noexcept
		{
			for (Reference<Type>* const pReference : other.m_spReferences)
				pReference->m_pReferencable = this;

			m_spReferences.merge(other.m_spReferences);

			return *this;
		}

		bool operator==(Reference<Type> const& reference)
		{
			return reference == this;
		}

	private:
		std::set<Reference<Type>*> m_spReferences{};
	};
}

#endif
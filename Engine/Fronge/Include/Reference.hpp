#if not defined fro_REFERENCE_H
#define fro_REFERENCE_H

#include <algorithm>
#include <utility>

namespace fro
{
	template<typename>
	class Referencable;

	template<typename Type>
	class Reference final
	{
		friend Referencable<Type>;

	public:
		Reference() = default;
		Reference(Referencable<Type>* const pReferencable)
			: m_pReferencable{ pReferencable }
		{
			if (valid())
				m_pReferencable->m_spReferences.insert(this);
		}

		Reference(Referencable<Type>& referencable)
			: Reference{ &referencable }
		{
		}

		Reference(Reference const& other)
			: m_pReferencable{ other.m_pReferencable }
		{
			if (valid())
				m_pReferencable->m_spReferences.insert(this);
		}

		Reference(Reference&& other) noexcept
			: Reference(other)
		{
			if (other.valid())
			{
				other.m_pReferencable->m_spReferences.erase(&other);
				other.m_pReferencable = nullptr;
			}
		}

		~Reference()
		{
			if (valid())
				m_pReferencable->m_spReferences.erase(this);
		}

		Reference& operator=(Reference const& other)
		{
			if (valid())
				m_pReferencable->m_spReferences.erase(this);

			m_pReferencable = other.m_pReferencable;

			if (valid())
				m_pReferencable->m_spReferences.insert(this);

			return *this;
		}

		Reference& operator=(Reference&& other) noexcept
		{
			*this = other;

			if (other.valid())
			{
				other.m_pReferencable->m_spReferences.erase(&other);
				other.m_pReferencable = nullptr;
			}

			return *this;
		}

		Reference& operator=(Type data)
		{
			get() = std::move(data);
			return *this;
		}

		bool operator==(Reference const& other) const
		{
			return m_pReferencable == other.m_pReferencable;
		}

		bool operator==(Referencable<Type> const* const pReferencable) const
		{
			return m_pReferencable == pReferencable;
		}

		bool operator==(Referencable<Type> const& referencable) const
		{
			return m_pReferencable == &referencable;
		}

		bool operator<(Reference const& other) const
		{
			return m_pReferencable < other.m_pReferencable;
		}

		bool valid() const
		{
			return m_pReferencable;
		}

		Type& get() const
		{
			return *static_cast<Type*>(m_pReferencable);
		}

	private:
		Referencable<Type>* m_pReferencable{};
	};
}

#endif
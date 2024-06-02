#if not defined fro_REFERENCABLE_H
#define fro_REFERENCABLE_H

#include "BaseReferencable.h"

namespace fro
{
	template<typename Type, template<typename> class Template>
	concept SpecializedType =
		std::_Is_specialization_v<Type, Template>;

	template<typename Type>
	class Referencable final : public BaseReferencable
	{
		static_assert(not DerivedReferencableType<Type>,
			"the held type cannot be derived from BaseReferencable");

		static_assert(not std::is_reference_v<Type>,
			"the held type cannot be a reference");

		template<typename>
		friend class Referencable;

	public:
		Referencable(Type data)
			: m_Data{ std::move(data) }
		{
		}

		template<typename... Arguments>
			requires std::constructible_from<Type, Arguments...>
		Referencable(Arguments&&... arguments)
			: m_Data{ std::forward<Arguments>(arguments)... }
		{
		}

		template<typename OtherType>
			requires std::constructible_from<Type, OtherType>
		Referencable(Referencable<OtherType> const& other)
			: m_Data{ other.m_Data }
		{
		}

		Referencable(Referencable const& other)
			: m_Data{ other.m_Data }
		{
		}

		template<typename OtherType>
			requires std::constructible_from<Type, OtherType>
		Referencable(Referencable<OtherType>&& other) noexcept
			: BaseReferencable(std::move(other))
			, m_Data{ std::move(other.m_Data) }
		{
		}

		Referencable(Referencable&& other) noexcept
			: BaseReferencable(std::move(other))
			, m_Data{ std::move(other.m_Data) }
		{
		}

		virtual ~Referencable() override = default;

		Referencable& operator=(Referencable const& other)
		{
			if (this == &other)
				return *this;

			m_Data = other.m_Data;
			return *this;
		}

		Referencable& operator=(Referencable&& other) noexcept
		{
			if (this == &other)
				return *this;

			BaseReferencable::operator=(std::move(other));
			m_Data = std::move(other.m_Data);
			return *this;
		}

		Type& get()
		{
			return m_Data;
		}

		Type const& get() const
		{
			return m_Data;
		}

	private:
		Type m_Data;
	};

	template<typename ...Types>
	concept SpecializedReferencableType =
		(SpecializedType<Types, Referencable> and ...);
}

#endif
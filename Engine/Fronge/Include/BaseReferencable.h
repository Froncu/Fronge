#if not defined fro_BASE_REFERENCABLE_H
#define fro_BASE_REFERENCABLE_H

#include <concepts>
#include <set>

namespace fro
{
	class BaseReference;

	class BaseReferencable
	{
		friend BaseReference;

	public:
		virtual ~BaseReferencable();

	protected:
		BaseReferencable() = default;
		BaseReferencable(BaseReferencable const&) = delete;
		BaseReferencable(BaseReferencable&& other) noexcept;

		BaseReferencable& operator=(BaseReferencable const&) = delete;
		BaseReferencable& operator=(BaseReferencable&& other) noexcept;

	private:
		std::set<BaseReference*> m_spReferences{};
	};

	template<typename ...Types>
	concept DerivedReferencableType =
		(std::derived_from<Types, BaseReferencable> and ...);
}

#endif
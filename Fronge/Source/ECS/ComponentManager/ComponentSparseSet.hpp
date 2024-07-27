#if not defined COMPONENT_SPARSE_SET_HPP
#define COMPONENT_SPARSE_SET_HPP

#include "ECS/Component/Component.hpp"
#include "Utility/SparseSet.hpp"

namespace fro
{
	struct BaseComponentSparseSet
	{
	public:
		virtual ~BaseComponentSparseSet() = default;

	protected:
		BaseComponentSparseSet() = default;
	};

	template<typename Type>
	concept ComponentSparseSetStorable =
		SparseSetStorable<Type> and
		std::derived_from<Type, Component>;

	template<ComponentSparseSetStorable ComponentType>
	struct ComponentSparseSet final : public BaseComponentSparseSet
	{
	public:
		virtual ~ComponentSparseSet() override = default;

		SparseSet<ComponentType> sparseSet{};
	};
}

#endif
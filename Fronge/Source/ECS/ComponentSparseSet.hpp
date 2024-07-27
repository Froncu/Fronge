#if not defined COMPONENT_SPARSE_SET_HPP
#define COMPONENT_SPARSE_SET_HPP

namespace fro
{
	class BaseComponentSparseSet
	{
	public:
		BaseComponentSparseSet() = default;
		BaseComponentSparseSet(BaseComponentSparseSet const&) = default;
		BaseComponentSparseSet(BaseComponentSparseSet&&) noexcept = default;

		virtual ~BaseComponentSparseSet() = default;

		BaseComponentSparseSet& operator=(BaseComponentSparseSet const&) = default;
		BaseComponentSparseSet& operator=(BaseComponentSparseSet&&) noexcept = default;
	};

	class ComponentSparseSet final : public BaseComponentSparseSet
	{

	};
}

#endif
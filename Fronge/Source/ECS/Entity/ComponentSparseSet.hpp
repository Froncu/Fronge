#if not defined COMPONENT_SPARSE_SET_HPP
#define COMPONENT_SPARSE_SET_HPP

#include "ECS/Components/Component.hpp"
#include "Utility/SparseSet.hpp"

namespace fro
{
	class BaseComponentSparseSet
	{
	public:
		virtual ~BaseComponentSparseSet() = default;

		virtual std::unique_ptr<Component> detach(std::size_t const entityID) = 0;

	protected:
		BaseComponentSparseSet() = default;
	};

	template<typename Type>
	concept ComponentSparseSetStorable =
		SparseSetStorable<Type> and
		std::derived_from<Type, Component>;

	template<ComponentSparseSetStorable ComponentType>
	class ComponentSparseSet final : public BaseComponentSparseSet
	{
	public:
		virtual ~ComponentSparseSet() override = default;

		virtual std::unique_ptr<Component> detach(std::size_t const entityID) override
		{
			std::optional<ComponentType> component{ mSparseSet.erase(entityID) };
			if (not component.has_value())
				return {};

			return std::make_unique<ComponentType>(std::move(*component));
		}

		SparseSet<ComponentType> mSparseSet{};
	};
}

#endif
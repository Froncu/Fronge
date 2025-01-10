#ifndef VARIANT_VISITOR_HPP
#define VARIANT_VISITOR_HPP

#include "froch.hpp"

namespace fro
{
	template<typename... CallableTypes>
	class VariantVisitor final
	{
		struct Visitor final : public CallableTypes...
		{
			using CallableTypes::operator()...;
		};

	public:
		VariantVisitor(CallableTypes... callables)
			: mVisitor{ callables... }
		{
		}

		VariantVisitor(VariantVisitor const&) = default;
		VariantVisitor(VariantVisitor&&) noexcept = default;

		~VariantVisitor() = default;

		VariantVisitor& operator=(VariantVisitor const&) = default;
		VariantVisitor& operator=(VariantVisitor&&) noexcept = default;

		template<typename VariantType>
		auto operator()(VariantType&& variant)
		{
			return std::visit(mVisitor, std::forward<VariantType>(variant));
		}

	private:
		Visitor mVisitor;
	};
}

#endif
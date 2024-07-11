#if not defined REFERENCABLE_HPP
#define REFERENCABLE_HPP

#include "froch.hpp"

#include "Core.hpp"

namespace fro
{
	class Referencable
	{
		friend class BaseReference;

	public:
		FRO_API virtual ~Referencable();

	protected:
		Referencable() = default;
		Referencable(Referencable&& other) noexcept;

		Referencable& operator=(Referencable&& other) noexcept;

	private:
		Referencable(Referencable const&) = delete;

		Referencable& operator=(Referencable const&) = delete;

		std::set<BaseReference*> mReferences{};
	};
}

#endif
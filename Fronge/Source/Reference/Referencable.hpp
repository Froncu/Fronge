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
		FRO_API Referencable(Referencable&& other) noexcept;

		FRO_API Referencable& operator=(Referencable&& other) noexcept;

	private:
		Referencable(Referencable const&) = delete;

		Referencable& operator=(Referencable const&) = delete;

		std::unordered_set<BaseReference*> mReferences{};
	};
}

#endif
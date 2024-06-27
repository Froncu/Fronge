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
		Referencable(Referencable const&);
		Referencable(Referencable&& other) noexcept;

		Referencable& operator=(Referencable const&);
		Referencable& operator=(Referencable&& other) noexcept;

	private:
		std::set<BaseReference*> mReferences{};
	};
}

#endif
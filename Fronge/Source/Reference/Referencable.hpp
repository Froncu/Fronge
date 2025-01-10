 #ifndef REFERENCABLE_HPP
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
		FRO_API Referencable(Referencable const&);
		FRO_API Referencable(Referencable&& other) noexcept;

		FRO_API Referencable& operator=(Referencable const&);
		FRO_API Referencable& operator=(Referencable&& other) noexcept;
		FRO_API FRO_NODISCARD virtual bool operator==(Referencable const&) const;
		FRO_API FRO_NODISCARD virtual std::strong_ordering operator<=>(Referencable const&) const;

	private:
		std::unordered_set<BaseReference*> mReferences{};
	};
}

#endif
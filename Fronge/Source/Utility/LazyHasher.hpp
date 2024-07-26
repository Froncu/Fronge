// Written based on Alx Sathskovsky's LinkedIn post
// (https://www.linkedin.com/pulse/generic-tuple-hashing-modern-c-alex-dathskovsky/)

#if not defined LAZY_HASHER_HPP
#define LAZY_HASHER_HPP

#include "froch.hpp"

namespace fro
{
	struct LazyHasher final
	{
		auto operator()(std::size_t hash, auto&&... values) const
		{
			auto lazyCombiner
			{
				[&hash](auto&& value)
				{
					return hash ^=
						std::hash<std::decay_t<decltype(value)>>{}(value)+
						0x9e3779b9 +
						(hash << 6) +
						(hash >> 2);
				}
			};

			(lazyCombiner(std::forward<decltype(values)>(values)), ...);

			return hash;
		}
	};
}

#endif
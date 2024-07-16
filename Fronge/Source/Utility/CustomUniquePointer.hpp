#if not defined UNIQUE_POINTER_HPP
#define UNIQUE_POINTER_HPP

#include "froch.hpp"

namespace fro
{
	template<typename ResourceType>
	using CustomUniquePointer = std::unique_ptr<ResourceType, std::function<void(ResourceType*)>>;
}

#endif
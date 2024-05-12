#if not defined fro_TYPENAMES_H
#define fro_TYPENAMES_H

#include <functional>
#include <memory>

namespace fro
{
	template<typename ResourceType>
	using CustomUniquePointer = std::unique_ptr<ResourceType, std::function<void(ResourceType*)>>;
}

#endif
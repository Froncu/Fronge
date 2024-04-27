#if not defined fro_TYPENAMES_H
#define fro_TYPENAMES_H

#include <memory>
#include <functional>

namespace fro
{
	template<typename ResourceType>
	using SDLUniquePointer = std::unique_ptr<ResourceType, std::function<void(ResourceType*)>>;
}

#endif
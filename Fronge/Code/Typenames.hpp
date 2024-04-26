#include <memory>
#include <functional>

namespace fro
{
	template<typename ResourceType>
	using SDLUniquePointer = std::unique_ptr<ResourceType, std::function<void(ResourceType*)>>;
}
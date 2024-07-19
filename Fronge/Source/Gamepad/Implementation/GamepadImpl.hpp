#if not defined GAMEPAD_IMPL_HPP
#define GAMEPAD_IMPL_HPP

#include "Gamepad/Gamepad.hpp"
#include "Utility/CustomUniquePointer.hpp"

struct _SDL_GameController;
typedef struct _SDL_GameController SDL_GameController;

namespace fro
{
	class Gamepad::Implementation final
	{
	public:
		Implementation(std::int32_t const deviceID);

		~Implementation() = default;

		FRO_NODISCARD SDL_GameController* getSDLGameController() const;

		FRO_NODISCARD std::int32_t getInstanceID() const;

	private:
		Implementation(Implementation const&) = delete;
		Implementation(Implementation&&) noexcept = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&&) noexcept = delete;

		CustomUniquePointer<SDL_GameController> mSDLGameController;
	};
}

#endif
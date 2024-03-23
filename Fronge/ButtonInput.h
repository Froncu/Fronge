#pragma once

#include <SDL2/SDL_Scancode.h>
#include <SDL2/SDL_gamecontroller.h>
#include <bitset>
#include <variant>
#include <optional>

namespace fro
{
	struct ButtonInput final
	{
	public:
		using Button = std::variant<SDL_Scancode, SDL_GameControllerButton>;

		enum class State
		{
			pressed = 0b01,
			released = 0b10,
			down = 0b11
		};

		ButtonInput(Button button, State state);
		ButtonInput(const ButtonInput&) = default;
		ButtonInput(ButtonInput&&) noexcept = default;

		~ButtonInput() = default;

		ButtonInput& operator=(const ButtonInput&) = default;
		ButtonInput& operator=(ButtonInput&&) noexcept = default;

		bool operator<(const ButtonInput& otherButtonInput) const;

		//template<typename ButtonType>
		//requires std::same_as<ButtonType, SDL_Scancode> or std::same_as<ButtonType, SDL_GameControllerButton>
		//std::optional<ButtonType> getButton() const
		//{
		//	if (std::holds_alternative<ButtonType>(m_Button))
		//		return std::get<ButtonType>(m_Button);

		//	return std::nullopt;
		//};

		const Button m_Button;
		const State m_State;
	};
}
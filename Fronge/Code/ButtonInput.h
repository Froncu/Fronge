#pragma once

#include "Defines.hpp"

#include <SDL_Scancode.h>
#include <SDL_gamecontroller.h>
#include <variant>
#include <optional>

namespace fro
{
	class ButtonInput final
	{
	public:
		using Button = std::variant<SDL_Scancode, SDL_GameControllerButton>;

		enum class State
		{
			pressed = 0b00,
			released = 0b01,
			down = 0b10
		};

		ButtonInput(Button button, State state);
		ButtonInput(const ButtonInput&) = default;
		ButtonInput(ButtonInput&&) noexcept = default;

		~ButtonInput() = default;

		ButtonInput& operator=(const ButtonInput&) = default;
		ButtonInput& operator=(ButtonInput&&) noexcept = default;

		fro_NODISCARD bool operator<(const ButtonInput& otherButtonInput) const;

		template<typename ButtonType>
		requires std::same_as<ButtonType, SDL_Scancode> or std::same_as<ButtonType, SDL_GameControllerButton>
		fro_NODISCARD_GETTER std::optional<ButtonType> getButton() const
		{
			if (std::holds_alternative<ButtonType>(m_Button))
				return std::get<ButtonType>(m_Button);

			return std::nullopt;
		};

		fro_NODISCARD_GETTER State getState() const;

	private:
		const Button m_Button;
		const State m_State;
	};
}
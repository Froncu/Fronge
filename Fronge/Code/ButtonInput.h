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

		ButtonInput(Button const button, State const state);
		ButtonInput(ButtonInput const&) = default;
		ButtonInput(ButtonInput&&) noexcept = default;

		~ButtonInput() = default;

		ButtonInput& operator=(ButtonInput const&) = default;
		ButtonInput& operator=(ButtonInput&&) noexcept = default;

		fro_NODISCARD bool operator<(ButtonInput const& otherButtonInput) const;

		fro_NODISCARD State getState() const;

		template<typename ButtonType>
			requires std::same_as<ButtonType, SDL_Scancode> or std::same_as<ButtonType, SDL_GameControllerButton>
		fro_NODISCARD std::optional<ButtonType> getButton() const
		{
			if (std::holds_alternative<ButtonType>(m_Button))
				return std::get<ButtonType>(m_Button);

			return std::nullopt;
		};

	private:
		Button const m_Button;
		State const m_State;
	};
}
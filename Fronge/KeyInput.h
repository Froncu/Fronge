#pragma once

#include <SDL2/SDL_Scancode.h>
#include <bitset>

namespace fro
{
	struct KeyInput final
	{
	public:
		enum class State
		{
			pressed = 0b01,
			released = 0b10,
			down = 0b11
		};

		KeyInput(SDL_Scancode key, State state);
		KeyInput(const KeyInput&) = default;
		KeyInput(KeyInput&&) noexcept = default;

		~KeyInput() = default;

		KeyInput& operator=(const KeyInput&) = default;
		KeyInput& operator=(KeyInput&&) noexcept = default;

		bool operator<(const KeyInput& otherKeyInput) const;

		const SDL_Scancode key;
		const State state;
	};
}
#pragma once

#include "Component.h"

namespace fro
{
	class GUI : public Component
	{
	public:
		virtual ~GUI() override = default;

		virtual void display() = 0;

	protected:
		GUI(const GameObject& parentingGameObject);

	private:
		GUI(const GUI&) = delete;
		GUI(GUI&&) noexcept = delete;

		GUI& operator=(const GUI&) = delete;
		GUI& operator=(GUI&&) noexcept = delete;
	};
}
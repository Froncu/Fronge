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
		GUI(GameObject const& parentingGameObject);

	private:
		GUI(GUI const&) = delete;
		GUI(GUI&&) noexcept = delete;

		GUI& operator=(GUI const&) = delete;
		GUI& operator=(GUI&&) noexcept = delete;
	};
}
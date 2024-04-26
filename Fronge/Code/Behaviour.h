#pragma once

#include "Component.h"

namespace fro
{
	class Behaviour : public Component
	{
	public:
		virtual ~Behaviour() override = default;

		virtual void update() = 0;

	protected:
		Behaviour(const GameObject& parentingGameObject);

	private:
		Behaviour(const Behaviour&) = delete;
		Behaviour(Behaviour&&) noexcept = delete;

		Behaviour& operator=(const Behaviour&) = delete;
		Behaviour& operator=(Behaviour&&) noexcept = delete;
	};
}
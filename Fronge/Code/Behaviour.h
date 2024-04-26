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
		Behaviour(GameObject const& parentingGameObject);

	private:
		Behaviour(Behaviour const&) = delete;
		Behaviour(Behaviour&&) noexcept = delete;

		Behaviour& operator=(Behaviour const&) = delete;
		Behaviour& operator=(Behaviour&&) noexcept = delete;
	};
}
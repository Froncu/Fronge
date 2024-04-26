#pragma once

#include "Component.h"

namespace fro
{
	class Renderable : public Component
	{
	public:
		virtual ~Renderable() override = default;

		virtual void render() const = 0;

	protected:
		Renderable(GameObject const& parentingGameObject);

	private:
		Renderable(Renderable const&) = delete;
		Renderable(Renderable&&) noexcept = delete;

		Renderable& operator=(Renderable const&) = delete;
		Renderable& operator=(Renderable&&) noexcept = delete;
	};
}
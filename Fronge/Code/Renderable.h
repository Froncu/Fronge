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
		Renderable(const GameObject& parentingGameObject);

	private:
		Renderable(const Renderable&) = delete;
		Renderable(Renderable&&) noexcept = delete;

		Renderable& operator=(const Renderable&) = delete;
		Renderable& operator=(Renderable&&) noexcept = delete;
	};
}
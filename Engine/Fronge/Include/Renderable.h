#if not defined fro_RENDERABLE_H
#define fro_RENDERABLE_H

#include "Component.h"

namespace fro
{
	class Renderable : public Component
	{
	public:
		virtual ~Renderable() override = default;

		virtual void render() const = 0;

	protected:
		Renderable(Reference<GameObject> const parentingGameObject);

	private:
		Renderable(Renderable const&) = delete;
		Renderable(Renderable&&) noexcept = delete;

		Renderable& operator=(Renderable const&) = delete;
		Renderable& operator=(Renderable&&) noexcept = delete;
	};
}

#endif
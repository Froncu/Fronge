#pragma once

#include "Component.h"

struct SDL_Renderer;

namespace fro
{
	class Renderable : public Component
	{
		friend GameObject;

	public:
		virtual ~Renderable() = default;

	protected:
		Renderable(const GameObject& parentingGameObject);

	private:
		Renderable(const Renderable&) = delete;
		Renderable(Renderable&&) noexcept = delete;

		Renderable& operator=(const Renderable&) = delete;
		Renderable& operator=(Renderable&&) noexcept = delete;

		virtual void render() const = 0;
	};
}

#ifndef fro_GENERATED_RENDERABLE_BODY
#define fro_GENERATED_RENDERABLE_BODY(identifier)\
fro_GENERATED_COMPONENT_BODY(identifier)\
\
private:\
	virtual void render() const override;
#endif

#ifndef fro_GENERATED_RENDERABLE_CONSTRUCTOR
#define fro_GENERATED_RENDERABLE_CONSTRUCTOR(identifier)\
fro::identifier::identifier(const GameObject& parentingGameObject)\
	: Renderable(parentingGameObject)
#endif

#ifndef fro_GENERATED_RENDERABLE_DESTRUCTOR
#define fro_GENERATED_RENDERABLE_DESTRUCTOR(identifier)\
fro_GENERATED_COMPONENT_DESTRUCTOR(identifier)
#endif

#ifndef fro_GENERATED_RENDERABLE_RENDER
#define fro_GENERATED_RENDERABLE_RENDER(identifier)\
void fro::identifier::render() const
#endif
// HACK: this whole approach of rendering using "renderable" components is not good.
// The engine should be able to render a predefined set of primitives, and that's it.
// This approach enforces each renderable component to figure out how to render itself,
// meaning taking the API used (OpenGL, SDL, Vulkan) into account etc.

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

		virtual void render(SDL_Renderer* const pRenderer) = 0;
	};
}

#ifndef fro_GENERATED_RENDERABLE_BODY
#define fro_GENERATED_RENDERABLE_BODY(identifier)\
friend GameObject;\
\
public:\
	virtual ~identifier() override; \
\
private:\
	identifier(const GameObject& parentingGameObject);\
	identifier(const identifier&) = delete;\
	identifier(identifier&&) noexcept = delete;\
\
	identifier& operator=(const identifier&) = delete;\
	identifier& operator=(identifier&&) noexcept = delete;\
\
	virtual void render(SDL_Renderer* const pRenderer) override;
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
void fro::identifier::render([[maybe_unused]] SDL_Renderer* const pRenderer)
#endif
// END HACK
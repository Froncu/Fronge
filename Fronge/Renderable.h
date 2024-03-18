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

// HACK: render() is not marked const; Plot needs this for now. Ideally, Plot should not be a Renderable but UI or something in this direction
#ifndef fro_GENERATED_RENDERABLE_BODY
#define fro_GENERATED_RENDERABLE_BODY(identifier, ...)\
namespace fro\
{\
	class identifier final : public Renderable\
	{\
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
		virtual void render(SDL_Renderer* const pRenderer) override;\
\
		__VA_ARGS__\
	};\
}
#endif

#ifndef fro_GENERATED_RENDERABLE_CONSTRUCTOR
#define fro_GENERATED_RENDERABLE_CONSTRUCTOR(identifier, ...)\
fro::identifier::identifier(const GameObject& parentingGameObject) :\
	Renderable(parentingGameObject)\
{\
	__VA_ARGS__\
}
#endif

#ifndef fro_GENERATED_RENDERABLE_DESTRUCTOR
#define fro_GENERATED_RENDERABLE_DESTRUCTOR(identifier, ...)\
fro_GENERATED_COMPONENT_DESTRUCTOR(identifier, __VA_ARGS__)
#endif

#ifndef fro_GENERATED_RENDERABLE_RENDER
#define fro_GENERATED_RENDERABLE_RENDER(identifier, body)\
void fro::identifier::render(SDL_Renderer* const pRenderer)\
{\
	body\
}
#endif
// END HACK
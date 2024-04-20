#pragma once

#include "Renderable.h"
#include "Defines.hpp"

#include <string>

struct SDL_Texture;
struct SDL_Rect;

namespace fro
{
	class Sprite final : public Renderable
	{
		fro_GENERATED_RENDERABLE_BODY(Sprite)

	public:
		void setFileName(const std::string& fileName);

	private:
		SDL_Texture* m_pTexture{};
	};
}
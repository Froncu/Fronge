#if not defined fro_SPRITE_H
#define fro_SPRITE_H

#include "Defines.hpp"
#include "Renderable.h"

#include <sdl_rect.h>

#include <xstring>

struct SDL_Texture;

namespace fro
{
	class Sprite final : public Renderable
	{
	public:
		Sprite(Reference<GameObject> const parentingGameObject);

		virtual ~Sprite() override = default;

		virtual void render() const override;

		void setFileName(std::string_view const fileName);
		void setSourceRectangle(SDL_FRect sourceRectangle);

		std::string_view getFileName() const;

	private:
		Sprite(Sprite const&) = delete;
		Sprite(Sprite&&) noexcept = delete;

		Sprite& operator=(Sprite const&) = delete;
		Sprite& operator=(Sprite&&) noexcept = delete;

		std::string_view m_FileName{};
		SDL_Texture* m_pTexture{};
		SDL_FRect m_SourceRectangle{};
	};
}

#endif
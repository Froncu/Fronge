#pragma once

#include "Renderable.h"
#include "Defines.hpp"

#include <string>

struct SDL_Texture;

namespace fro
{
	class Sprite final : public Renderable
	{
	public:
		Sprite(GameObject const& parentingGameObject);

		virtual ~Sprite() override = default;

		virtual void render() const override;

		void setFileName(std::string const& fileName);

	private:
		Sprite(Sprite const&) = delete;
		Sprite(Sprite&&) noexcept = delete;

		Sprite& operator=(Sprite const&) = delete;
		Sprite& operator=(Sprite&&) noexcept = delete;

		SDL_Texture* m_pTexture{};
	};
}
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
		Sprite(const GameObject& parentingGameObject);

		virtual ~Sprite() override = default;

		virtual void render() const override;

		void setFileName(const std::string& fileName);

	private:
		Sprite(const Sprite&) = delete;
		Sprite(Sprite&&) noexcept = delete;

		Sprite& operator=(const Sprite&) = delete;
		Sprite& operator=(Sprite&&) noexcept = delete;

		SDL_Texture* m_pTexture{};
	};
}
#if not defined fro_SPRITE_ANIMATOR_H
#define fro_SPRITE_ANIMATOR_H

#include "Behaviour.h"

#include <sdl_rect.h>
#include <vec2.hpp>

#include <map>
#include <vector>
#include <xstring>

namespace fro
{
	class SpriteAnimator final : public Behaviour
	{
	public:
		struct AnimationFrame final
		{
			std::string_view fileName;
			SDL_FRect sourceRectangle;
		};

		SpriteAnimator(GameObject const& parentingGameObject);

		virtual ~SpriteAnimator() override = default;

		virtual void update() override;
		void play();
		void reset();
		void pause();
		void addAnimationFrames(std::string_view const animationName,
			std::initializer_list<AnimationFrame> const& animationFrames);
		void addAnimationFrames(std::string_view const animationName,
			std::string_view const animationFileName,
			glm::vec2 const& animationFrameSize,
			std::uint32_t const amountOfCellsX,
			std::uint32_t const amountOfCellsY);
		void setFramesPerSecond(std::string_view const animationName, int const framesPerSecond);
		void setActiveAnimation(std::string_view const animationName);

	private:
		struct Animation final
		{
			std::vector<AnimationFrame> vAnimationFrames{};
			int currentFrameIndex{};

			float elapsedSeconds{};
			float frameTimeSeconds{ 1.0f };
		};

		SpriteAnimator(SpriteAnimator const&) = delete;
		SpriteAnimator(SpriteAnimator&&) noexcept = delete;

		SpriteAnimator& operator=(SpriteAnimator const&) = delete;
		SpriteAnimator& operator=(SpriteAnimator&&) noexcept = delete;

		void updateSprite();

		std::map<std::string_view, Animation> m_mAnimations{};
		Animation* m_pActiveAnimation{};

		bool m_Play{ true };
	};
}

#endif
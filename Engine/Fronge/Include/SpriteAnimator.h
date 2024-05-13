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

		SpriteAnimator(GameObject& parentingGameObject);

		virtual ~SpriteAnimator() override = default;

		virtual void update(float const deltaSeconds) override;

		void setActiveAnimation(std::string_view const animationName);
		void play();
		void pause();
		void reset();
		void stop();

		void addAnimationFrames(std::string_view const animationName,
			std::initializer_list<AnimationFrame> const& animationFrames);
		void addAnimationFrames(std::string_view const animationName,
			std::string_view const animationFileName,
			glm::vec2 const& animationFrameSize,
			std::uint32_t const amountOfCellsX,
			std::uint32_t const amountOfCellsY);
		void setFramesPerSecond(std::string_view const animationName, int const framesPerSecond);
		void setLoop(std::string_view const animationName, bool shouldLoop);

	private:
		struct Animation final
		{
			std::vector<AnimationFrame> vAnimationFrames{};
			float frameTimeSeconds{ 1.0f / 5 };
			bool shouldLoop{ true };
		};

		SpriteAnimator(SpriteAnimator const&) = delete;
		SpriteAnimator(SpriteAnimator&&) noexcept = delete;

		SpriteAnimator& operator=(SpriteAnimator const&) = delete;
		SpriteAnimator& operator=(SpriteAnimator&&) noexcept = delete;

		void updateSprite() const;

		std::map<std::string_view, Animation> m_mAnimations{};
		Animation* m_pActiveAnimation{};

		int m_CurrentFrameIndex{};
		float m_ElapsedSeconds{};
		bool m_Play{ true };
	};
}

#endif
#if not defined fro_SPRITE_ANIMATOR_H
#define fro_SPRITE_ANIMATOR_H

#include "Behaviour.h"
#include "Event.hpp"

#include <sdl_rect.h>
#include <vec2.hpp>

#include <map>
#include <vector>
#include <xstring>

namespace fro
{
	class Sprite;

	class SpriteAnimator final : public Behaviour
	{
	public:
		struct AnimationFrame final
		{
			std::string fileName;
			SDL_FRect sourceRectangle;
		};

		struct Animation final
		{
			std::vector<AnimationFrame> vAnimationFrames{};
			float frameTimeSeconds{ 1.0f / 5 };
			bool shouldLoop{ true };
		};

		SpriteAnimator(Reference<GameObject> const parentingGameObject);

		virtual ~SpriteAnimator() override = default;

		virtual void awake() override;

		virtual void update(float const deltaSeconds) override;

		void setActiveAnimation(std::string const& animationName);
		void play();
		void pause();
		void reset();
		void stop();

		void addAnimationFrames(std::string const& animationName,
			std::initializer_list<AnimationFrame> animationFrames);
		void addAnimationFrames(std::string const& animationName,
			std::string const& animationFileName,
			glm::vec2 const& animationFrameSize,
			std::uint32_t const amountOfCellsX,
			std::uint32_t const amountOfCellsY);
		void setFramesPerSecond(std::string const& animationName, int const framesPerSecond);
		void setLoop(std::string const& animationName, bool shouldLoop);

		Animation const& getAnimation(std::string const& animationName);
		float getAnimationProgress() const;

		std::size_t getCurrentFrameIndex() const;

		Event<std::size_t> frameChanged{};
		Event<> animationFinished{};

	private:
		SpriteAnimator(SpriteAnimator const&) = delete;
		SpriteAnimator(SpriteAnimator&&) noexcept = delete;

		SpriteAnimator& operator=(SpriteAnimator const&) = delete;
		SpriteAnimator& operator=(SpriteAnimator&&) noexcept = delete;

		void updateSprite() const;

		Reference<Sprite> m_Sprite{};

		std::map<std::string, Animation> m_mAnimations{};
		Animation* m_pActiveAnimation{};

		std::size_t m_CurrentFrameIndex{};
		float m_ElapsedSeconds{};
		bool m_Play{ true };
	};
}

#endif
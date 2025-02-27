#include "froch.hpp"

#include "SpriteAnimatorSystem.hpp"

namespace fro
{
	void SpriteAnimatorSystem::onUpdate(double const deltaSeconds)
	{
		for (auto&& [entity, sprite, spriteAnimator] : sGroup)
		{
			bool& play{ spriteAnimator->play };
			Reference<SpriteAnimation> const& animation{ spriteAnimator->animation };
			if (not play or not animation.valid())
				return;

			auto& sourceRectangles{ animation->sourceRectangles };
			if (sourceRectangles.empty())
				return;

			double& frameProgress{ spriteAnimator->frameProgress };
			double const framesPerSecond{ animation->framesPerSecond };
			frameProgress += deltaSeconds * framesPerSecond;
			if (frameProgress < 1.0)
				return;

			std::size_t& frameIndex{ spriteAnimator->frameIndex };
			if (frameIndex == sourceRectangles.size() - 1 and not animation->loops)
			{
				frameProgress = 1.0;
				spriteAnimator->play = false;
				return;
			}

			--frameProgress;

			++frameIndex %= sourceRectangles.size();

			sprite->sourceRectangle = sourceRectangles[frameIndex];
		}
	}


	Group<Sprite, SpriteAnimator> SpriteAnimatorSystem::sGroup{};
}
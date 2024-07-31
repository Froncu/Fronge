#include "froch.hpp"

#include "SpriteAnimatorSystem.hpp"

namespace fro
{
	void SpriteAnimatorSystem::onUpdate(double const deltaSeconds)
	{
		for (auto&& [entity, sprite, spriteAnimation] : sGroup)
		{
			bool& play{ spriteAnimation->play };
			Reference<SpriteAnimationClip> const& animationClip{ spriteAnimation->animationClip };
			if (not play or not animationClip.valid())
				return;

			auto& sourceRectangles{ animationClip->sourceRectangles };
			if (sourceRectangles.empty())
				return;

			double& elapsedSeconds{ spriteAnimation->elapsedSeconds };
			double const frameTimeSeconds{ animationClip->frameTimeSeconds };
			elapsedSeconds += deltaSeconds;
			if (elapsedSeconds < frameTimeSeconds)
				return;

			std::size_t& currentFrameIndex{ spriteAnimation->currentFrameIndex };
			if (currentFrameIndex == sourceRectangles.size() - 1 and not animationClip->shouldLoop)
			{
				elapsedSeconds = frameTimeSeconds;
				spriteAnimation->play = false;
				return;
			}

			elapsedSeconds -= frameTimeSeconds;

			++currentFrameIndex %= sourceRectangles.size();

			sprite->sourceRectangle = sourceRectangles[currentFrameIndex];
		}
	}

	Group<Sprite, SpriteAnimation> SpriteAnimatorSystem::sGroup{};
}
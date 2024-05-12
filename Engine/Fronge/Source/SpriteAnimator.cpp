#include "SpriteAnimator.h"

#include "GameObject.h"
#include "Sprite.h"
#include "Timer.h"

#pragma region Constructors/Destructor
fro::SpriteAnimator::SpriteAnimator(GameObject const& parentingGameObject)
	: Behaviour(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::SpriteAnimator::update()
{
	if (not m_Play or not m_pActiveAnimation)
		return;

	auto& [vAnimationFrames, currentFrameIndex, elapsedSeconds, frameTimeSeconds]
		{ *m_pActiveAnimation };

	if (not vAnimationFrames.size())
		return;

	elapsedSeconds += Timer::getInstance().getDeltaSeconds();
	if (elapsedSeconds < frameTimeSeconds)
		return;

	elapsedSeconds -= frameTimeSeconds;
	++currentFrameIndex %= vAnimationFrames.size();

	updateSprite();
}

void fro::SpriteAnimator::play()
{
	m_Play = true;
}

void fro::SpriteAnimator::reset()
{
	if (not m_pActiveAnimation)
		return;

	auto& [vAnimationFrames, currentFrameIndex, elapsedSeconds, frameTimeSeconds]
		{ *m_pActiveAnimation };

	currentFrameIndex = 0;
	elapsedSeconds = 0.0f;
}

void fro::SpriteAnimator::pause()
{
	m_Play = false;
}

void fro::SpriteAnimator::addAnimationFrames(std::string_view const animationName,
	std::initializer_list<AnimationFrame> const& animationFrames)
{
	Animation& animation{ m_mAnimations[animationName] };
	auto& vAnimationFrames{ animation.vAnimationFrames };
	vAnimationFrames.insert(vAnimationFrames.end(), animationFrames);

	if (not m_pActiveAnimation)
		m_pActiveAnimation = &animation;

	updateSprite();
}

void fro::SpriteAnimator::addAnimationFrames(std::string_view const animationName,
	std::string_view const animationFileName,
	glm::vec2 const& animationFrameSize,
	std::uint32_t const amountOfFramesX,
	std::uint32_t const amountOfFramesY)
{
	Animation& animation{ m_mAnimations[animationName] };

	for (std::uint32_t y{}; y < amountOfFramesY; ++y)
		for (std::uint32_t x{}; x < amountOfFramesX; ++x)
			animation.vAnimationFrames.push_back(AnimationFrame(animationFileName,
				SDL_FRect
				{
					.x{ x * animationFrameSize.x },
					.y{ y * animationFrameSize.y },
					.w{ animationFrameSize.x },
					.h{ animationFrameSize.y }
				}));

	if (not m_pActiveAnimation)
		m_pActiveAnimation = &animation;

	updateSprite();
}

void fro::SpriteAnimator::setFramesPerSecond(std::string_view const animationName, int const framesPerSecond)
{
	m_mAnimations[animationName].frameTimeSeconds = 1.0f / framesPerSecond;
}

void fro::SpriteAnimator::setActiveAnimation(std::string_view const animationName)
{
	m_pActiveAnimation = &m_mAnimations[animationName];
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::SpriteAnimator::updateSprite()
{
	AnimationFrame const& currentAnimationFrame{ 
		m_pActiveAnimation->vAnimationFrames[m_pActiveAnimation->currentFrameIndex] };
	Sprite& spriteComponent{ *getParentingGameObject().getComponent<Sprite>() };

	if (spriteComponent.getFileName() not_eq currentAnimationFrame.fileName)
		spriteComponent.setFileName(currentAnimationFrame.fileName);

	spriteComponent.setSourceRectangle(currentAnimationFrame.sourceRectangle);
}
#pragma endregion PrivateMethods
#include "SpriteAnimator.h"

#pragma region Constructors/Destructor
fro::SpriteAnimator::SpriteAnimator(Reference<GameObject> const parentingGameObject)
	: Behaviour(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::SpriteAnimator::update(float const deltaSeconds)
{
	if (not m_Play or not m_pActiveAnimation)
		return;

	auto& [vAnimationFrames, frameTimeSeconds, shouldLoop] { *m_pActiveAnimation };

	if (vAnimationFrames.empty())
		return;

	m_ElapsedSeconds += deltaSeconds;
	if (m_ElapsedSeconds < frameTimeSeconds)
		return;

	if (m_CurrentFrameIndex == vAnimationFrames.size() - 1 and not shouldLoop)
	{
		m_ElapsedSeconds = frameTimeSeconds;
		m_Play = false;
		animationFinished.notifySubscribers();
		return;
	}

	m_ElapsedSeconds -= frameTimeSeconds;

	++m_CurrentFrameIndex %= vAnimationFrames.size();
	updateSprite();

	frameChanged.notifySubscribers(m_CurrentFrameIndex);
}

void fro::SpriteAnimator::setActiveAnimation(std::string const& animationName)
{
	Animation& animation{ m_mAnimations[animationName] };
	if (m_pActiveAnimation not_eq &animation)
	{
		m_pActiveAnimation = &animation;
		reset();
	}
}

void fro::SpriteAnimator::play()
{
	m_Play = true;
}

void fro::SpriteAnimator::pause()
{
	m_Play = false;
}

void fro::SpriteAnimator::reset()
{
	m_CurrentFrameIndex = 0;
	updateSprite();

	frameChanged.notifySubscribers(m_CurrentFrameIndex);

	m_ElapsedSeconds = 0.0f;
}

void fro::SpriteAnimator::stop()
{
	pause();
	reset();
}

void fro::SpriteAnimator::addAnimationFrames(std::string const& animationName,
	std::initializer_list<AnimationFrame> animationFrames)
{
	Animation& animation{ m_mAnimations[animationName] };
	auto& vAnimationFrames{ animation.vAnimationFrames };
	vAnimationFrames.insert(vAnimationFrames.end(), std::move(animationFrames));

	if (not m_pActiveAnimation)
	{
		m_pActiveAnimation = &animation;
		updateSprite();
	}
}

void fro::SpriteAnimator::addAnimationFrames(std::string const& animationName,
	std::string const& animationFileName,
	glm::vec2 const& animationFrameSize,
	std::uint32_t const amountOfFramesX,
	std::uint32_t const amountOfFramesY)
{
	Animation& animation{ m_mAnimations[animationName] };

	for (std::uint32_t y{}; y < amountOfFramesY; ++y)
		for (std::uint32_t x{}; x < amountOfFramesX; ++x)
			animation.vAnimationFrames.push_back(AnimationFrame
				{
					.fileName{ animationFileName },
					.sourceRectangle
					{
						.x{ x * animationFrameSize.x },
						.y{ y * animationFrameSize.y },
						.w{ animationFrameSize.x },
						.h{ animationFrameSize.y }
					}
				});

	if (not m_pActiveAnimation)
	{
		m_pActiveAnimation = &animation;
		updateSprite();
	}
}

void fro::SpriteAnimator::setFramesPerSecond(std::string const& animationName, int const framesPerSecond)
{
	m_mAnimations[animationName].frameTimeSeconds = 1.0f / framesPerSecond;
}

void fro::SpriteAnimator::setLoop(std::string const& animationName, bool const shouldLoop)
{
	m_mAnimations[animationName].shouldLoop = shouldLoop;
}

fro::SpriteAnimator::Animation const& fro::SpriteAnimator::getAnimation(std::string const& animationName)
{
	return m_mAnimations[animationName];
}

float fro::SpriteAnimator::getAnimationProgress() const
{
	if (not m_pActiveAnimation)
		return 0.0f;

	return
		(m_pActiveAnimation->frameTimeSeconds * m_CurrentFrameIndex + m_ElapsedSeconds) /
		(m_pActiveAnimation->vAnimationFrames.size() * m_pActiveAnimation->frameTimeSeconds);
}

std::size_t fro::SpriteAnimator::getCurrentFrameIndex() const
{
	if (not m_pActiveAnimation)
		return 0;

	return m_CurrentFrameIndex;
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::SpriteAnimator::updateSprite() const
{
	AnimationFrame const& currentAnimationFrame
	{
		m_pActiveAnimation->vAnimationFrames[m_CurrentFrameIndex]
	};

	if (m_Sprite.get().getFileName() not_eq currentAnimationFrame.fileName)
		m_Sprite.get().setFileName(currentAnimationFrame.fileName);

	m_Sprite.get().setSourceRectangle(currentAnimationFrame.sourceRectangle);
}
#pragma endregion PrivateMethods
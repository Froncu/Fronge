#include "Fronge.h"

#include "GUIContext.h"
#include "InputManager.h"
#include "PhysicsManager.h"
#include "RenderContext.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Steam.h"
#include "SystemEventManager.h"

#include <sdl.h>
#include <vld.h>

#include <chrono>

#pragma region Constructors/Destructor
fro::Fronge::Fronge()
{
	SystemEventManager::getInstance().m_SystemEvent.addSubscribers({
		std::bind(&InputManager::processInputEvent, &InputManager::getInstance(), std::placeholders::_1),
		std::bind(&GUIContext::processSystemEvent, &GUIContext::getInstance(), std::placeholders::_1),
		std::bind(&RenderContext::processSystemEvent, &RenderContext::getInstance(), std::placeholders::_1) });
}

fro::Fronge::~Fronge()
{
	SDL_Quit();
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
int fro::Fronge::run() const
{
	auto oldTimePoint{ std::chrono::steady_clock::now() };
	float fixedUpdateElapsedSeconds{};

	while (true)
	{
		Steam::getInstance().update();

		auto const currentTimePoint{ std::chrono::steady_clock::now() };
		float const deltaSeconds{ std::chrono::duration<float>(currentTimePoint - oldTimePoint).count() };
		oldTimePoint = currentTimePoint;

		InputManager::getInstance().processInputContinous();
		if (not SystemEventManager::getInstance().processSystemEvents())
			break;

		for (fixedUpdateElapsedSeconds += deltaSeconds;
			fixedUpdateElapsedSeconds >= m_FixedDeltaSeconds;
			fixedUpdateElapsedSeconds -= m_FixedDeltaSeconds)
		{
			PhysicsManager::getInstance().update(m_FixedDeltaSeconds);
			SceneManager::getInstance().fixedUpdate(m_FixedDeltaSeconds);
		}

		SceneManager::getInstance().update(deltaSeconds);

		RenderContext::getInstance().clear();
		SceneManager::getInstance().render();

		GUIContext::getInstance().startFrame();
		SceneManager::getInstance().display();

		GUIContext::getInstance().endFrame();
		RenderContext::getInstance().present();
	}

	return 0;
}

void fro::Fronge::setFixedFrameRate(int const fixedFrameRate)
{
	m_FixedDeltaSeconds = 1.0f / fixedFrameRate;
}
#pragma endregion PublicMethods
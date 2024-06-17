#include "Application.hpp"
#include "SystemEventManager/SystemEventManager.hpp"

#include <SDL.h>

#include <iostream>

namespace fro
{
	void Application::run()
	{
		while (true)
		{
			SystemEventManager::pollEvents();
		}
	}
}
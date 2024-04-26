#pragma once

#include "Singleton.hpp"

typedef union SDL_Event SDL_Event;

namespace fro
{
	class GUIContext final : public Singleton<GUIContext>
	{
	public:
		GUIContext();

		virtual ~GUIContext() override;

		void processSystemEvent(const SDL_Event& event) const;
		void startFrame() const;
		void endFrame() const;

	private:
		GUIContext(const GUIContext&) = delete;
		GUIContext(GUIContext&&) noexcept = delete;

		GUIContext& operator=(const GUIContext&) = delete;
		GUIContext& operator=(GUIContext&&) noexcept = delete;
	};
}
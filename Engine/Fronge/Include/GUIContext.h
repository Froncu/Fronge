#if not defined fro_GUI_CONTEXT_H
#define fro_GUI_CONTEXT_H

#include "Singleton.hpp"

typedef union SDL_Event SDL_Event;

namespace fro
{
	class GUIContext final : public Singleton<GUIContext>
	{
		friend class Singleton<GUIContext>;

	public:
		virtual ~GUIContext() override;

		void processSystemEvent(SDL_Event const& event) const;
		void startFrame() const;
		void endFrame() const;

	private:
		GUIContext();
		GUIContext(GUIContext const&) = delete;
		GUIContext(GUIContext&&) noexcept = delete;

		GUIContext& operator=(GUIContext const&) = delete;
		GUIContext& operator=(GUIContext&&) noexcept = delete;
	};
}

#endif
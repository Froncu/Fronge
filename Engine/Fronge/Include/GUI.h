#if not defined fro_GUI_H
#define fro_GUI_H

#include "Component.h"

namespace fro
{
	class GUI : public Component
	{
	public:
		virtual ~GUI() override = default;

		virtual void display() = 0;

	protected:
		GUI(GameObject& parentingGameObject);

	private:
		GUI(GUI const&) = delete;
		GUI(GUI&&) noexcept = delete;

		GUI& operator=(GUI const&) = delete;
		GUI& operator=(GUI&&) noexcept = delete;
	};
}

#endif
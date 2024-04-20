#pragma once

#include "Component.h"

namespace fro
{
	class GUI : public Component
	{
		friend GameObject;

	public:
		virtual ~GUI() = default;

	protected:
		GUI(const GameObject& parentingGameObject);

	private:
		GUI(const GUI&) = delete;
		GUI(GUI&&) noexcept = delete;

		GUI& operator=(const GUI&) = delete;
		GUI& operator=(GUI&&) noexcept = delete;

		virtual void display() = 0;
	};
}

#ifndef fro_GENERATED_GUI_BODY
#define fro_GENERATED_GUI_BODY(identifier)\
fro_GENERATED_COMPONENT_BODY(identifier)\
\
private:\
	virtual void display() override;
#endif

#ifndef fro_GENERATED_GUI_CONSTRUCTOR
#define fro_GENERATED_GUI_CONSTRUCTOR(identifier)\
fro::identifier::identifier(const GameObject& parentingGameObject)\
	: GUI(parentingGameObject)
#endif

#ifndef fro_GENERATED_GUI_DESTRUCTOR
#define fro_GENERATED_GUI_DESTRUCTOR(identifier)\
fro_GENERATED_COMPONENT_DESTRUCTOR(identifier)
#endif

#ifndef fro_GENERATED_GUI_DISPLAY
#define fro_GENERATED_GUI_DISPLAY(identifier)\
void fro::identifier::display()
#endif
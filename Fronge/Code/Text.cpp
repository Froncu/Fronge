#include "Text.h"

#include "ResourceManager.h"
#include "RenderContext.h"
#include "GameObject.h"

#include <SDL_render.h>

#pragma region Constructors/Destructor
fro::Text::Text(const GameObject& parentingGameObject) :
	Renderable(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Text::render() const
{
	RenderContext::getInstance().renderTexture(m_pTexture, getParentingGameObject().getComponent<Transform>()->getWorldPosition());
}

void fro::Text::setFont(const std::string& fontName, int fontSize)
{
	m_FontName = fontName;
	m_FontSize = fontSize;

	updateTexture();
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::Text::updateTexture()
{
	m_pTexture = ResourceManager::getInstance().getTextTexture(RenderContext::getInstance().getRenderer(), m_FontName, m_FontSize, m_Text);
}
#pragma endregion PrivateMethods
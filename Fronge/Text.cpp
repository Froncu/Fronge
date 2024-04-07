#include "Text.h"

#include "ResourceManager.h"
#include "RenderContext.h"
#include "GameObject.h"

#include <SDL2/SDL_render.h>

#pragma region Constructors/Destructor
fro_GENERATED_RENDERABLE_CONSTRUCTOR(Text)
{
}

fro_GENERATED_RENDERABLE_DESTRUCTOR(Text)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Text::setFont(const std::string& fontName, int fontSize)
{
	m_FontName = fontName;
	m_FontSize = fontSize;

	updateTexture();
}

SDL_Texture* fro::Text::getTexture() const
{
	return m_pTexture;
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
fro_GENERATED_RENDERABLE_RENDER(Text)
{
	RenderContext::getInstance().renderTexture(m_pTexture, getParentingGameObject().getComponent<Transform>()->getWorldPosition());
}

void fro::Text::updateTexture()
{
	m_pTexture = ResourceManager::getInstance().getTextTexture(RenderContext::getInstance().getRenderer(), m_FontName, m_FontSize, m_Text);
}
#pragma endregion PrivateMethods
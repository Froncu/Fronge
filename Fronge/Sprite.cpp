#include "Sprite.h"

#include "ResourceManager.h"
#include "RenderContext.h"
#include "GameObject.h"

#include <SDL2/SDL_render.h>

#pragma region Constructors/Destructor
fro_GENERATED_RENDERABLE_CONSTRUCTOR(Sprite)
{
}

fro_GENERATED_RENDERABLE_DESTRUCTOR(Sprite)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Sprite::setFileName(const std::string& fileName)
{
	m_pTexture = ResourceManager::getInstance().getImageTexture(RenderContext::getInstance().getRenderer(), fileName);
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
fro_GENERATED_RENDERABLE_RENDER(Sprite)
{
	RenderContext::getInstance().renderTexture(m_pTexture, getParentingGameObject().getComponent<Transform>()->getWorldPosition());
}
#pragma endregion PrivateMethods
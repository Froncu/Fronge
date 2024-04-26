#include "Sprite.h"

#include "ResourceManager.h"
#include "RenderContext.h"
#include "GameObject.h"

#include <SDL_render.h>

#pragma region Constructors/Destructor
fro::Sprite::Sprite(const GameObject& parentingGameObject) :
	Renderable(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Sprite::render() const
{
	RenderContext::getInstance().renderTexture(m_pTexture, getParentingGameObject().getComponent<Transform>()->getWorldPosition());
}

void fro::Sprite::setFileName(const std::string& fileName)
{
	m_pTexture = ResourceManager::getInstance().getImageTexture(RenderContext::getInstance().getRenderer(), fileName);
}
#pragma endregion PublicMethods
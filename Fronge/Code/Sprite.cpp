#include "Sprite.h"

#include "GameObject.h"
#include "RenderContext.h"
#include "ResourceManager.h"

#include <sdl_render.h>

#pragma region Constructors/Destructor
fro::Sprite::Sprite(GameObject const& parentingGameObject)
	: Renderable(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Sprite::render() const
{
	RenderContext::getInstance().renderTexture(m_pTexture, getParentingGameObject().getComponent<Transform>()->getWorldPosition());
}

void fro::Sprite::setFileName(std::string const& fileName)
{
	m_pTexture = ResourceManager::getInstance().getImageTexture(RenderContext::getInstance().getRenderer(), fileName);
}
#pragma endregion PublicMethods
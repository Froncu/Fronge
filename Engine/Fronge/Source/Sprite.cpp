#include "Sprite.h"

#include "GameObject.h"
#include "RenderContext.h"
#include "ResourceManager.h"

#include <sdl_render.h>

#pragma region Constructors/Destructor
fro::Sprite::Sprite(GameObject& parentingGameObject)
	: Renderable(parentingGameObject)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Sprite::render() const
{
	RenderContext::getInstance().renderTexture(m_pTexture,
		getParentingGameObject().getComponent<Transform>()->getWorldTransform(),
		m_SourceRectangle);
}

void fro::Sprite::setFileName(std::string_view const fileName)
{
	m_FileName = fileName;
	m_pTexture = ResourceManager::getInstance().getImageTexture(RenderContext::getInstance().getRenderer(), m_FileName);
}

void fro::Sprite::setSourceRectangle(SDL_FRect sourceRectangle)
{
	m_SourceRectangle = sourceRectangle;
}

std::string_view fro::Sprite::getFileName() const 
{
	return m_FileName;
}
#pragma endregion PublicMethods
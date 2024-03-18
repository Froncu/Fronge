#include "Sprite.h"

#include "ResourceManager.h"
#include "GameObject.h"
#include "Transform.h"

#include <SDL2/SDL_render.h>

#pragma region Constructors/Destructor
fro_GENERATED_RENDERABLE_CONSTRUCTOR(Sprite)
fro_GENERATED_RENDERABLE_DESTRUCTOR(Sprite)
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::Sprite::setFileName(const std::string& fileName)
{
	m_FileName = fileName;
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
fro_GENERATED_RENDERABLE_RENDER
(
	Sprite,

	const glm::vec2& worldPosition{ getParentingGameObject().getComponent<Transform>()->getWorldPosition()};
	SDL_Rect destinationRectangle;
	
	destinationRectangle.x = static_cast<int>(worldPosition.x);
	destinationRectangle.y = static_cast<int>(worldPosition.y);

	SDL_Texture* const pTexture{ fro::ResourceManager::getImageTexture(pRenderer, m_FileName.c_str()) };
	SDL_QueryTexture(pTexture, nullptr, nullptr, &destinationRectangle.w, &destinationRectangle.h);

	SDL_RenderCopy(pRenderer, pTexture, nullptr, &destinationRectangle);
)
#pragma endregion PrivateMethods
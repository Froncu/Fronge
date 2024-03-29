#include "Text.h"

#include "ResourceManager.h"
#include "GameObject.h"
#include "Transform.h"

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
void fro::Text::setFont(const std::string& fileName, int size)
{
	m_Font = { fileName, size };
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
fro_GENERATED_RENDERABLE_RENDER(Text)
{
	// HACK: this is duplicate code compared to the Sprite
	SDL_Texture* const pTexture{ fro::ResourceManager::getInstance().getTextTexture(pRenderer, m_Font.first.c_str(), m_Font.second, m_Text) };
	SDL_Rect destinationRectangle;
	SDL_QueryTexture(pTexture, nullptr, nullptr, &destinationRectangle.w, &destinationRectangle.h);

	const glm::vec2& worldPosition{ getParentingGameObject().getComponent<Transform>()->getWorldPosition() };
	destinationRectangle.x = static_cast<int>(worldPosition.x) - destinationRectangle.w / 2;
	destinationRectangle.y = static_cast<int>(worldPosition.y) - destinationRectangle.h / 2;

	SDL_RenderCopy(pRenderer, pTexture, nullptr, &destinationRectangle);
	// END HACK
}
#pragma endregion PrivateMethods
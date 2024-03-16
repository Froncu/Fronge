#include "ResourceManager.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <cassert>

#pragma region StaticDataMembers
std::string fro::ResourceManager::m_RESOURCES_DIRECTORY{ "../Resources/" };
std::unordered_map<std::pair<std::string, int>, fro::ResourceManager::SDLUniquePointer<TTF_Font>, fro::ResourceManager::PairHash, fro::ResourceManager::PairEqual> fro::ResourceManager::m_mpFONTS{};
std::unordered_map<std::pair<std::string, int>, std::unordered_map<std::string, fro::ResourceManager::SDLUniquePointer<SDL_Texture>>, fro::ResourceManager::PairHash, fro::ResourceManager::PairEqual> fro::ResourceManager::m_mmpTEXT_TEXTURE_MAPS{};
std::unordered_map < std::string, fro::ResourceManager::SDLUniquePointer<SDL_Texture>> fro::ResourceManager::m_mpIMAGE_TEXTURES{};
#pragma endregion StaticDataMembers



#pragma region PublicMethods
fro_NODISCARD_GETTER SDL_Texture* const fro::ResourceManager::getTextTexture(SDL_Renderer* const pRenderer, const std::string& fileName, int size, const std::string& text)
{
	auto& mpTextTextures{ m_mmpTEXT_TEXTURE_MAPS[{ fileName, size }]};

	auto iterator{ mpTextTextures.find(text) };

	if (iterator == mpTextTextures.end())
	{
		TTF_Font* const pFont{ getFont(fileName, size) };
		SDLUniquePointer<SDL_Surface> pTextSurface{ TTF_RenderText_Blended(pFont, text.c_str(), SDL_Color(255, 255, 255, 255)), SDL_FreeSurface };
		SDLUniquePointer<SDL_Texture> pTextTexture{ SDL_CreateTextureFromSurface(pRenderer, pTextSurface.get()), SDL_DestroyTexture };

		auto resultPair{ mpTextTextures.insert({ text, std::move(pTextTexture) }) };
		return resultPair.first->second.get();
	}
	
	return iterator->second.get();
}

fro_NODISCARD_GETTER SDL_Texture* const fro::ResourceManager::getImageTexture(SDL_Renderer* const pRenderer, const std::string& imageFileName)
{
	auto& pTexture{ m_mpIMAGE_TEXTURES[imageFileName] };

	if (!pTexture.get())
		pTexture = SDLUniquePointer<SDL_Texture>(IMG_LoadTexture(pRenderer, (m_RESOURCES_DIRECTORY + imageFileName).c_str()), SDL_DestroyTexture);

	return pTexture.get();
}

void fro::ResourceManager::setResourcesDirectory(const std::string& resourcesDirectory)
{
	m_RESOURCES_DIRECTORY = resourcesDirectory;
}

void fro::ResourceManager::clearCaches()
{
	m_mpFONTS.clear();
	m_mmpTEXT_TEXTURE_MAPS.clear();
	m_mpIMAGE_TEXTURES.clear();
}
#pragma region PublicMethods



#pragma region PrivateMethods
fro_NODISCARD_GETTER TTF_Font* const fro::ResourceManager::getFont(const std::string& fileName, int size)
{
	auto& pFont{ m_mpFONTS[{ fileName, size }] };

	if (!pFont.get())
		pFont = SDLUniquePointer<TTF_Font>(TTF_OpenFont((m_RESOURCES_DIRECTORY + fileName).c_str(), size), TTF_CloseFont);

	return pFont.get();
}
#pragma endregion PrivateMethods
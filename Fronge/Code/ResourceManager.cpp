#include "ResourceManager.h"

#include <sdl_image.h>
#include <sdl_mixer.h>
#include <sdl_ttf.h>

#include <format>
#include <stdexcept>

#pragma region Constructors/Destructor
fro::ResourceManager::ResourceManager()
{
	if (constexpr int imageFlags{ NULL }; IMG_Init(imageFlags) != imageFlags)
		throw std::runtime_error(std::format("[ IMG_Init FAILED ] -> {}", IMG_GetError()));

	if (TTF_Init() == -1)
		throw std::runtime_error(std::format("[ TTF_Init FAILED ] -> {}", TTF_GetError()));
}

fro::ResourceManager::~ResourceManager()
{
	clearCaches(); // TODO: the caches need to be cleared before quiting the subsystems, I would prefer this to not be necessary

	TTF_Quit();
	IMG_Quit();
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void fro::ResourceManager::clearCaches()
{
	m_mpFonts.clear();
	m_mmpTextTexturesMap.clear();
	m_mpImageTextures.clear();
	m_mpAudioMusics.clear();
	m_mpAudioEffects.clear();
}

void fro::ResourceManager::setResourcesDirectory(std::string const& resourcesDirectory)
{
	m_ResourcesDirectory = resourcesDirectory;
}

SDL_Texture* fro::ResourceManager::getTextTexture(SDL_Renderer* const pRenderer, std::string const& fileName, int const size, std::string const& text)
{
	auto& mpTextTextures{ m_mmpTextTexturesMap[{ fileName, size }] };

	auto const iterator{ mpTextTextures.find(text) };

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

SDL_Texture* fro::ResourceManager::getImageTexture(SDL_Renderer* const pRenderer, std::string const& imageFileName)
{
	auto& pTexture{ m_mpImageTextures[imageFileName] };

	if (!pTexture.get())
		pTexture = SDLUniquePointer<SDL_Texture>(IMG_LoadTexture(pRenderer, (m_ResourcesDirectory + imageFileName).c_str()), SDL_DestroyTexture);

	return pTexture.get();
}

Mix_Music* fro::ResourceManager::getMusic(std::string const& audioFileName)
{
	auto& pMusic{ m_mpAudioMusics[audioFileName] };

	if (!pMusic.get())
		pMusic = SDLUniquePointer<Mix_Music>(Mix_LoadMUS((m_ResourcesDirectory + audioFileName).c_str()), Mix_FreeMusic);

	return pMusic.get();
}

Mix_Chunk* fro::ResourceManager::getEffect(std::string const& audioFileName)
{
	auto& pEffect{ m_mpAudioEffects[audioFileName] };

	if (!pEffect.get())
		pEffect = SDLUniquePointer<Mix_Chunk>(Mix_LoadWAV((m_ResourcesDirectory + audioFileName).c_str()), Mix_FreeChunk);

	return pEffect.get();
}
#pragma region PublicMethods



#pragma region PrivateMethods
TTF_Font* fro::ResourceManager::getFont(std::string const& fileName, int const size)
{
	auto& pFont{ m_mpFonts[{ fileName, size }] };

	if (!pFont.get())
		pFont = SDLUniquePointer<TTF_Font>(TTF_OpenFont((m_ResourcesDirectory + fileName).c_str(), size), TTF_CloseFont);

	return pFont.get();
}
#pragma endregion PrivateMethods
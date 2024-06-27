#include "froch.hpp"

#include "Logger/Logger.hpp"
#include "ResourceManager.hpp"

#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

namespace fro
{
	std::unordered_map<std::string, CustomUniquePointer<SDL_Texture>> ResourceManager::mImageTextures{};
	std::unordered_map<std::string, CustomUniquePointer<Mix_Music>> ResourceManager::mAudioMusics{};
	std::unordered_map<std::string, CustomUniquePointer<Mix_Chunk>> ResourceManager::mAudioEffects{};

	std::string ResourceManager::mResourcesDirectory{ "Resources/" };

	std::map<ResourceManager::FontInfo, CustomUniquePointer<TTF_Font>> ResourceManager::mFonts{};

	void ResourceManager::initialize()
	{
		if (int constexpr imageFlags{ NULL }; IMG_Init(imageFlags) not_eq imageFlags)
			throw std::runtime_error(std::format("IMG_Init() failed: {}", IMG_GetError()));

		if (TTF_Init() == -1)
			throw std::runtime_error(std::format("TTF_Init() failed: {}", TTF_GetError()));

		fro::Logger::info("ResourceManager intialized!");
	}

	void ResourceManager::shutDown()
	{
		clearCaches(); // TODO: the caches need to be cleared before quiting the subsystems, I would prefer this to not be necessary

		TTF_Quit();
		IMG_Quit();

		fro::Logger::info("ResourceManager shut down!");
	}

	void fro::ResourceManager::clearCaches()
	{
		mFonts.clear();
		mImageTextures.clear();
		mAudioMusics.clear();
		mAudioEffects.clear();
	}

	void fro::ResourceManager::setResourcesDirectory(std::string resourcesDirectory)
	{
		mResourcesDirectory = std::move(resourcesDirectory);
	}

	fro::CustomUniquePointer<SDL_Texture> fro::ResourceManager::getTextTexture(SDL_Renderer* const pRenderer, std::string const& fileName, int const size, std::string const& text)
	{
		TTF_Font* const pFont{ getFont(fileName, size) };

		CustomUniquePointer<SDL_Surface> pTextSurface
		{
			TTF_RenderText_Blended(pFont, text.data(),
				SDL_Color(255, 255, 255, 255)), SDL_FreeSurface
		};

		return
		{
			SDL_CreateTextureFromSurface(pRenderer, pTextSurface.get()),
			SDL_DestroyTexture
		};
	}

	SDL_Texture* fro::ResourceManager::getImageTexture(SDL_Renderer* const pRenderer, std::string const& imageFileName)
	{
		auto& pTexture{ mImageTextures[imageFileName] };

		if (not pTexture.get())
			pTexture = CustomUniquePointer<SDL_Texture>(IMG_LoadTexture(pRenderer, std::format("{}{}", mResourcesDirectory, imageFileName).c_str()), SDL_DestroyTexture);

		return pTexture.get();
	}

	Mix_Music* fro::ResourceManager::getMusic(std::string const& audioFileName)
	{
		auto& pMusic{ mAudioMusics[audioFileName] };

		if (not pMusic.get())
			pMusic = CustomUniquePointer<Mix_Music>(Mix_LoadMUS(std::format("{}{}", mResourcesDirectory, audioFileName).c_str()), Mix_FreeMusic);

		return pMusic.get();
	}

	Mix_Chunk* fro::ResourceManager::getEffect(std::string const& audioFileName)
	{
		auto& pEffect{ mAudioEffects[audioFileName] };

		if (not pEffect.get())
			pEffect = CustomUniquePointer<Mix_Chunk>(Mix_LoadWAV(std::format("{}{}", mResourcesDirectory, audioFileName).c_str()), Mix_FreeChunk);

		return pEffect.get();
	}

	TTF_Font* fro::ResourceManager::getFont(std::string const& fileName, int const size)
	{
		auto& pFont{ mFonts[{ fileName, size }] };

		if (not pFont.get())
			pFont = CustomUniquePointer<TTF_Font>(TTF_OpenFont(std::format("{}{}", mResourcesDirectory, fileName).c_str(), size), TTF_CloseFont);

		return pFont.get();
	}
}
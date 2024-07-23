#include "froch.hpp"

#include "Logger/Logger.hpp"
#include "ResourceManager.hpp"

namespace fro
{
	Reference<Font> ResourceManager::findFont(std::string name)
	{
		auto const& pair{ sFonts.find(name) };
		if (pair == sFonts.end())
			return nullptr;

		return pair->second;
	}

	Reference<Music> ResourceManager::findMusic(std::string name)
	{
		auto const& pair{ sMusics.find(name) };
		if (pair == sMusics.end())
			return nullptr;

		return pair->second;
	}

	Reference<SoundEffect> ResourceManager::findSoundEffect(std::string name)
	{
		auto const& pair{ sSoundEffect.find(name) };
		if (pair == sSoundEffect.end())
			return nullptr;

		return pair->second;
	}

	Reference<Surface> ResourceManager::findSurface(std::string name)
	{
		auto const& pair{ sSurfaces.find(name) };
		if (pair == sSurfaces.end())
			return nullptr;

		return pair->second;
	}

	Reference<Texture> ResourceManager::findTexture(std::string name)
	{
		auto const& pair{ sTextures.find(name) };
		if (pair == sTextures.end())
			return nullptr;

		return pair->second;
	}

	std::unordered_map<std::string, Font> ResourceManager::sFonts{};
	std::unordered_map<std::string, Music> ResourceManager::sMusics{};
	std::unordered_map<std::string, SoundEffect> ResourceManager::sSoundEffect{};
	std::unordered_map<std::string, Surface> ResourceManager::sSurfaces{};
	std::unordered_map<std::string, Texture> ResourceManager::sTextures{};
}
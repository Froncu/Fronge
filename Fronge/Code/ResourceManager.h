#pragma once

#include "Singleton.hpp"
#include "Defines.hpp"
#include "Typenames.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

struct SDL_Renderer;
struct SDL_Texture;
typedef struct _Mix_Music Mix_Music;
struct Mix_Chunk;
typedef struct _TTF_Font TTF_Font;

namespace fro
{
	// TODO: this whole class feels messy
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		ResourceManager();

		virtual ~ResourceManager() override;

		void clearCaches();

		void setResourcesDirectory(const std::string& resourcesDirectory);

		fro_NODISCARD SDL_Texture* getTextTexture(SDL_Renderer* const pRenderer, const std::string& fileName, int size, const std::string& text);
		fro_NODISCARD SDL_Texture* getImageTexture(SDL_Renderer* const pRenderer, const std::string& imageFileName);
		fro_NODISCARD Mix_Music* getMusic(const std::string& audioFileName);
		fro_NODISCARD Mix_Chunk* getEffect(const std::string& audioFileName);

	private:
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) noexcept = delete;

		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) noexcept = delete;

		struct PairHash
		{
			template <class Type1, class Type2>
			std::size_t operator()(const std::pair<Type1, Type2>& pair) const
			{
				const auto hash1{ std::hash<Type1>{}(pair.first) };
				const auto hash2{ std::hash<Type2>{}(pair.second) };
				return hash1 ^ hash2;
			}
		};

		struct PairEqual
		{
			template <class Type1, class Type2>
			bool operator()(const std::pair<Type1, Type2>& pair1, const std::pair<Type1, Type2>& pair2) const
			{
				return pair1.first == pair2.first && pair1.second == pair2.second;
			}
		};

		fro_NODISCARD TTF_Font* getFont(const std::string& fileName, int size);

		std::string m_ResourcesDirectory{ "Resources/" };
		std::unordered_map<std::pair<std::string, int>, SDLUniquePointer<TTF_Font>, PairHash, PairEqual> m_mpFonts{};
		std::unordered_map<std::pair<std::string, int>, std::unordered_map<std::string, SDLUniquePointer<SDL_Texture>>, PairHash, PairEqual> m_mmpTextTexturesMap{};
		std::unordered_map<std::string, SDLUniquePointer<SDL_Texture>> m_mpImageTextures{};
		std::unordered_map<std::string, SDLUniquePointer<Mix_Music>> m_mpAudioMusics{};
		std::unordered_map<std::string, SDLUniquePointer<Mix_Chunk>> m_mpAudioEffects{};
	};
	// END TODO
}
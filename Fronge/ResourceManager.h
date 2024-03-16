#pragma once

#include "Defines.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

struct SDL_Renderer;
struct SDL_Texture;
typedef struct _TTF_Font TTF_Font;

namespace fro
{
	class ResourceManager final
	{
		friend class Fronge;

	public:
		static fro_NODISCARD_GETTER SDL_Texture* const getTextTexture(SDL_Renderer* const pRenderer, const std::string& fileName, int size, const std::string& text);
		static fro_NODISCARD_GETTER SDL_Texture* const getImageTexture(SDL_Renderer* const pRenderer, const std::string& imageFileName);
		
		static void setResourcesDirectory(const std::string& resourcesDirectory);
		static void clearCaches();

	private:
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

		ResourceManager() = delete;
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager(ResourceManager&&) noexcept = delete;

		~ResourceManager() = delete;

		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) noexcept = delete;

		template<typename ResourceType>
		using SDLUniquePointer = std::unique_ptr<ResourceType, std::function<void(ResourceType*)>>;

		static fro_NODISCARD_GETTER TTF_Font* const getFont(const std::string& fileName, int size);

		static std::string m_RESOURCES_DIRECTORY;
		static std::unordered_map<std::pair<std::string, int>, SDLUniquePointer<TTF_Font>, PairHash, PairEqual> m_mpFONTS;
		static std::unordered_map<std::pair<std::string, int>, std::unordered_map<std::string, SDLUniquePointer<SDL_Texture>>, PairHash, PairEqual> m_mmpTEXT_TEXTURE_MAPS;
		static std::unordered_map<std::string, SDLUniquePointer<SDL_Texture>> m_mpIMAGE_TEXTURES;
	};
}
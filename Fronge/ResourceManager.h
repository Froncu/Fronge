#pragma once

#include "Singleton.hpp"
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
	class ResourceManager final : public Singleton<ResourceManager>
	{
		fro_GENERATED_SINGLETON_BODY(ResourceManager)

	public:
		fro_NODISCARD_GETTER SDL_Texture* getTextTexture(SDL_Renderer* const pRenderer, const std::string& fileName, int size, const std::string& text);
		fro_NODISCARD_GETTER SDL_Texture* getImageTexture(SDL_Renderer* const pRenderer, const std::string& imageFileName);
		
		void setResourcesDirectory(const std::string& resourcesDirectory);
		void clearCaches();

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

		template<typename ResourceType>
		using SDLUniquePointer = std::unique_ptr<ResourceType, std::function<void(ResourceType*)>>;

		fro_NODISCARD_GETTER TTF_Font* getFont(const std::string& fileName, int size);

		std::string m_ResourcesDirectory{ "../Resources/" };
		std::unordered_map<std::pair<std::string, int>, SDLUniquePointer<TTF_Font>, PairHash, PairEqual> m_mpFonts{};
		std::unordered_map<std::pair<std::string, int>, std::unordered_map<std::string, SDLUniquePointer<SDL_Texture>>, PairHash, PairEqual> m_mmpTextTexturesMap{};
		std::unordered_map<std::string, SDLUniquePointer<SDL_Texture>> m_mpImageTextures{};
	};
}
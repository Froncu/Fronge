#if not defined fro_RESOURCE_MANAGER_H
#define fro_RESOURCE_MANAGER_H

#include "Defines.hpp"
#include "Singleton.hpp"
#include "Typenames.hpp"

#include <functional>
#include <memory>
#include <xstring>
#include <unordered_map>

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

		void setResourcesDirectory(std::string_view const resourcesDirectory);

		fro_NODISCARD SDL_Texture* getTextTexture(SDL_Renderer* const pRenderer, std::string_view const fileName, int const size, std::string_view const text);
		fro_NODISCARD SDL_Texture* getImageTexture(SDL_Renderer* const pRenderer, std::string_view const imageFileName);
		fro_NODISCARD Mix_Music* getMusic(std::string_view const audioFileName);
		fro_NODISCARD Mix_Chunk* getEffect(std::string_view const audioFileName);

	private:
		ResourceManager(ResourceManager const&) = delete;
		ResourceManager(ResourceManager&&) noexcept = delete;

		ResourceManager& operator=(ResourceManager const&) = delete;
		ResourceManager& operator=(ResourceManager&&) noexcept = delete;

		struct PairHash
		{
			template <class Type1, class Type2>
			std::size_t operator()(std::pair<Type1, Type2> const& pair) const
			{
				auto const hash1{ std::hash<Type1>{}(pair.first) };
				auto const hash2{ std::hash<Type2>{}(pair.second) };
				return hash1 ^ hash2;
			}
		};

		struct PairEqual
		{
			template <class Type1, class Type2>
			bool operator()(std::pair<Type1, Type2> const& pair1, std::pair<Type1, Type2> const& pair2) const
			{
				return pair1.first == pair2.first && pair1.second == pair2.second;
			}
		};

		fro_NODISCARD TTF_Font* getFont(std::string_view const fileName, int const size);

		std::string_view m_ResourcesDirectory{ "Resources/" };
		std::unordered_map<std::pair<std::string_view, int>, SDLUniquePointer<TTF_Font>, PairHash, PairEqual> m_mpFonts{};
		std::unordered_map<std::pair<std::string_view, int>, std::unordered_map<std::string_view, SDLUniquePointer<SDL_Texture>>, PairHash, PairEqual> m_mmpTextTexturesMap{};
		std::unordered_map<std::string_view, SDLUniquePointer<SDL_Texture>> m_mpImageTextures{};
		std::unordered_map<std::string_view, SDLUniquePointer<Mix_Music>> m_mpAudioMusics{};
		std::unordered_map<std::string_view, SDLUniquePointer<Mix_Chunk>> m_mpAudioEffects{};
	};
	// END TODO
}

#endif
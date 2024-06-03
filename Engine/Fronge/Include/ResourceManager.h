#if not defined fro_RESOURCE_MANAGER_H
#define fro_RESOURCE_MANAGER_H

#include "Defines.hpp"
#include "Singleton.hpp"
#include "Typenames.hpp"

#include <functional>
#include <map>
#include <memory>
#include <string>
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
		friend class Singleton<ResourceManager>;

	public:
		virtual ~ResourceManager() override;

		void clearCaches();

		void setResourcesDirectory(std::string resourcesDirectory);

		fro_NODISCARD CustomUniquePointer<SDL_Texture> getTextTexture(SDL_Renderer* const pRenderer, std::string const& fileName, int const size, std::string const& text);
		fro_NODISCARD SDL_Texture* getImageTexture(SDL_Renderer* const pRenderer, std::string const& imageFileName);
		fro_NODISCARD Mix_Music* getMusic(std::string const& audioFileName);
		fro_NODISCARD Mix_Chunk* getEffect(std::string const& audioFileName);

	private:
		ResourceManager();
		ResourceManager(ResourceManager const&) = delete;
		ResourceManager(ResourceManager&&) noexcept = delete;

		ResourceManager& operator=(ResourceManager const&) = delete;
		ResourceManager& operator=(ResourceManager&&) noexcept = delete;

		fro_NODISCARD TTF_Font* getFont(std::string const& fileName, int const size);

		std::unordered_map<std::string, CustomUniquePointer<SDL_Texture>> m_mpImageTextures{};
		std::unordered_map<std::string, CustomUniquePointer<Mix_Music>> m_mpAudioMusics{};
		std::unordered_map<std::string, CustomUniquePointer<Mix_Chunk>> m_mpAudioEffects{};

		std::string m_ResourcesDirectory{ "Resources/" };

		using FontInfo = std::pair<std::string, int>;
		std::map<FontInfo, CustomUniquePointer<TTF_Font>> m_mpFonts{};
	};
	// END TODO
}

#endif
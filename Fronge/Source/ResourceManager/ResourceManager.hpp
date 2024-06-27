#if not defined fro_RESOURCE_MANAGER_HPP
#define fro_RESOURCE_MANAGER_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Utility.hpp"

struct SDL_Renderer;
struct SDL_Texture;
typedef struct _Mix_Music Mix_Music;
struct Mix_Chunk;
typedef struct _TTF_Font TTF_Font;

namespace fro
{
	class ResourceManager final
	{
		using FontInfo = std::pair<std::string, int>;

	public:
		FRO_API static void initialize();
		FRO_API static void shutDown();

		FRO_API static void clearCaches();

		FRO_API static void setResourcesDirectory(std::string resourcesDirectory);

		FRO_API FRO_NODISCARD static CustomUniquePointer<SDL_Texture> getTextTexture(SDL_Renderer* const pRenderer, std::string const& fileName, int const size, std::string const& text);
		FRO_API FRO_NODISCARD static SDL_Texture* getImageTexture(SDL_Renderer* const pRenderer, std::string const& imageFileName);
		FRO_API FRO_NODISCARD static Mix_Music* getMusic(std::string const& audioFileName);
		FRO_API FRO_NODISCARD static Mix_Chunk* getEffect(std::string const& audioFileName);

	private:
		FRO_NODISCARD static TTF_Font* getFont(std::string const& fileName, int const size);

		static std::unordered_map<std::string, CustomUniquePointer<SDL_Texture>> mImageTextures;
		static std::unordered_map<std::string, CustomUniquePointer<Mix_Music>> mAudioMusics;
		static std::unordered_map<std::string, CustomUniquePointer<Mix_Chunk>> mAudioEffects;

		static std::string mResourcesDirectory;

		static std::map<FontInfo, CustomUniquePointer<TTF_Font>> mFonts;

		ResourceManager() = delete;
		ResourceManager(ResourceManager const&) = delete;
		ResourceManager(ResourceManager&&) noexcept = delete;

		~ResourceManager() = delete;

		ResourceManager& operator=(ResourceManager const&) = delete;
		ResourceManager& operator=(ResourceManager&&) noexcept = delete;
	};
}

#endif
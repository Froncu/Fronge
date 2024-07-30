#if not defined RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "froch.hpp"

#include "Core.hpp"
#include "Logger/Logger.hpp"
#include "Resources/Font.hpp"
#include "Resources/Music.hpp"
#include "Resources/SoundEffect.hpp"
#include "Resources/Surface.hpp"
#include "Resources/Texture.hpp"

namespace fro
{
	class ResourceManager final
	{
	public:
		FRO_API FRO_NODISCARD static Font* findFont(std::string name);
		FRO_API FRO_NODISCARD static Music* findMusic(std::string name);
		FRO_API FRO_NODISCARD static SoundEffect* findSoundEffect(std::string name);
		FRO_API FRO_NODISCARD static Surface* findSurface(std::string name);
		FRO_API FRO_NODISCARD static Texture* findTexture(std::string name);

		template<typename... ArgumentTypes>
			requires std::constructible_from<Font, ArgumentTypes...>
		static Font& storeFont(std::string name, ArgumentTypes&&... arguments)
		{
			auto&& [resource, didInsert] { sSurfaces.insert({ std::move(name), Font{ std::forward<ArgumentTypes>(arguments)... } }) };
			if (didInsert)
			{
				Logger::info("stored Font with ID {} in the ResourceManager!",
					resource->second.getID());
			}
			else
			{
				Logger::warn("failed to store Font with ID {} in the ResourceManager (it's already stored)",
					resource->second.getID());
			}

			return resource->second;
		}

		template<typename... ArgumentTypes>
			requires std::constructible_from<Music, ArgumentTypes...>
		static Music& storeMusic(std::string name, ArgumentTypes&&... arguments)
		{
			auto&& [resource, didInsert] { sSurfaces.insert({ std::move(name), Music{ std::forward<ArgumentTypes>(arguments)... } }) };
			if (didInsert)
			{
				Logger::info("stored Music with ID {} in the ResourceManager!",
					resource->second.getID());
			}
			else
			{
				Logger::warn("failed to store Music with ID {} in the ResourceManager (it's already stored)",
					resource->second.getID());
			}

			return resource->second;
		}

		template<typename... ArgumentTypes>
			requires std::constructible_from<SoundEffect, ArgumentTypes...>
		static SoundEffect& storeSoundEffect(std::string name, ArgumentTypes&&... arguments)
		{
			auto&& [resource, didInsert] { sSurfaces.insert({ std::move(name), SoundEffect{ std::forward<ArgumentTypes>(arguments)... } }) };
			if (didInsert)
			{
				Logger::info("stored SoundEffect with ID {} in the ResourceManager!",
					resource->second.getID());
			}
			else
			{
				Logger::warn("failed to store SoundEffect with ID {} in the ResourceManager (it's already stored)",
					resource->second.getID());
			}

			return resource->second;
		}

		template<typename... ArgumentTypes>
			requires std::constructible_from<Surface, ArgumentTypes...>
		static Surface& storeSurface(std::string name, ArgumentTypes&&... arguments)
		{
			auto&& [resource, didInsert] { sSurfaces.insert({ std::move(name), Surface{ std::forward<ArgumentTypes>(arguments)... } }) };
			if (didInsert)
			{
				Logger::info("stored Surface with ID {} in the ResourceManager!",
					resource->second.getID());
			}
			else
			{
				Logger::warn("failed to store Surface with ID {} in the ResourceManager (it's already stored)",
					resource->second.getID());
			}

			return resource->second;
		}

		template<typename... ArgumentTypes>
			requires std::constructible_from<Texture, ArgumentTypes...>
		static Texture& storeTexture(std::string name, ArgumentTypes&&... arguments)
		{
			auto&& [resource, didInsert] { sTextures.insert({ std::move(name), Texture{ std::forward<ArgumentTypes>(arguments)... } }) };
			if (didInsert)
			{
				Logger::info("stored Texture with ID {} in the ResourceManager!",
					resource->second.getID());
			}
			else
			{
				Logger::warn("failed to store Texture with ID {} in the ResourceManager (it's already stored)",
					resource->second.getID());
			}

			return resource->second;
		}

	private:
		FRO_API static std::unordered_map<std::string, Font> sFonts;
		FRO_API static std::unordered_map<std::string, Music> sMusics;
		FRO_API static std::unordered_map<std::string, SoundEffect> sSoundEffect;
		FRO_API static std::unordered_map<std::string, Surface> sSurfaces;
		FRO_API static std::unordered_map<std::string, Texture> sTextures;

		ResourceManager() = delete;
		ResourceManager(ResourceManager const&) = delete;
		ResourceManager(ResourceManager&&) noexcept = delete;

		~ResourceManager() = delete;

		ResourceManager& operator=(ResourceManager const&) = delete;
		ResourceManager& operator=(ResourceManager&&) noexcept = delete;
	};
}

#endif
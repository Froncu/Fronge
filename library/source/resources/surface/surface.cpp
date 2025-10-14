#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

#include "froch.hpp"
#include "surface.hpp"

namespace fro
{
   // TODO: make a better hash
   Surface::Surface(std::filesystem::path const& image_path)
      : native_surface_{
         [&image_path]
         {
            if (not std::filesystem::is_regular_file(image_path))
               throw std::runtime_error{ std::format("\"{}\" is not a file", image_path.string()) };

            SDL_Surface* const native_surface{ IMG_Load(image_path.string().c_str()) };
            if (not native_surface)
               throw std::runtime_error{
                  std::format("failed to load \"{}\" into an Surface ({})",
                     image_path.filename().string(), SDL_GetError())
               };

            return native_surface;
         }(),
         SDL_DestroySurface
      }
      , hash_{
         std::hash<std::string>{}(std::string(static_cast<char*>(native_surface_->pixels),
            native_surface_->pitch * native_surface_->h))
      }
   {
   }

   Vector2<int> Surface::size() const
   {
      return { native_surface_->w, native_surface_->h };
   }

   std::size_t Surface::hash() const
   {
      return hash_;
   }
}
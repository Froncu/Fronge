#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

#include "froch.hpp"
#include "surface.hpp"

namespace fro
{
   Surface::Surface(std::string_view const image_path)
      : native_surface_{
         [image_path]
         {
            SDL_Surface* surface{ IMG_Load(image_path.data()) };
            if (not surface)
               exception("failed to load \"{}\" into an SDL_Surface ({})",
                  image_path, SDL_GetError());

            return surface;
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
}
#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "core.hpp"
#include "froch.hpp"
#include "maths/vector2.hpp"
#include "utility/unique_pointer.hpp"

struct SDL_Surface;

namespace fro
{
   class Renderer;
   class Texture;

   class Surface final
   {
      friend Renderer;
      friend Texture;

      public:
         FRO_API explicit Surface(std::string_view image_path);
         Surface(Surface const&) = delete;
         Surface(Surface&&) = default;

         ~Surface() = default;

         Surface& operator=(Surface const&) = delete;
         Surface& operator=(Surface&&) = default;

         FRO_API [[nodiscard]] Vector2<int> size() const;

      private:
         UniquePointer<SDL_Surface> native_surface_;
         std::size_t hash_;
   };
}

#endif
#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "fronge/api.hpp"
#include "fronge/pch.hpp"
#include "fronge/unique_pointer.hpp"
#include "fronge/vector2.hpp"

struct SDL_Surface;

namespace fro
{
   class Texture;

   class Surface final
   {
      friend Texture;

      public:
         FRO_API explicit Surface(std::filesystem::path const& image_path);
         Surface(Surface const&) = delete;
         Surface(Surface&&) = default;

         ~Surface() = default;

         Surface& operator=(Surface const&) = delete;
         Surface& operator=(Surface&&) = default;

         FRO_API [[nodiscard]] Vector2<int> size() const;
         FRO_API [[nodiscard]] std::size_t hash() const;

      private:
         UniquePointer<SDL_Surface> native_surface_;
         std::size_t hash_;
   };
}

#endif
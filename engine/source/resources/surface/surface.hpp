#ifndef SURFACE_HPP
#define SURFACE_HPP

#include "core.hpp"
#include "froch.hpp"
#include "maths/vector2.hpp"
#include "reference/referenceable.hpp"
#include "utility/unique_pointer.hpp"

struct SDL_Surface;

namespace fro
{
   class Texture;

   class Surface final : public Referenceable
   {
      friend Texture;

      public:
         FRO_API explicit Surface(std::string_view image_path);
         Surface(Surface const&) = delete;
         Surface(Surface&&) = default;

         virtual ~Surface() override = default;

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
#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "core.hpp"
#include "froch.hpp"
#include "texture.hpp"
#include "utility/unique_pointer.hpp"

struct SDL_Renderer;

namespace fro
{
   class Surface;
   class Window;

   class Renderer final
   {
      friend Texture;

      public:
         FRO_API explicit Renderer(Window const& window);
         Renderer(Renderer const&) = delete;
         Renderer(Renderer&&) = default;

         FRO_API ~Renderer() = default;

         Renderer& operator=(Renderer const&) = delete;
         Renderer& operator=(Renderer&&) = default;

         FRO_API [[nodiscard]] Texture const& upload_texture(Surface const& surface);

      private:
         UniquePointer<SDL_Renderer> native_renderer_;
         std::unordered_map<std::size_t, Texture> textures_{};
   };
}

#endif
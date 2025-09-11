#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "core.hpp"
#include "events/observer/event_dispatcher.hpp"
#include "froch.hpp"
#include "maths/shapes.hpp"
#include "maths/transform_matrix/transform_matrix.hpp"
#include "maths/vector2.hpp"
#include "reference/reference.hpp"
#include "reference/referenceable.hpp"
#include "texture.hpp"
#include "utility/unique_pointer.hpp"
#include "window/window.hpp"

struct SDL_Window;
struct SDL_Renderer;

namespace fro
{
   struct Color final
   {
      std::uint8_t red;
      std::uint8_t green;
      std::uint8_t blue;
      std::uint8_t alpha;
   };

   class EditorUI;

   class Renderer final : public Referenceable
   {
      friend EditorUI;

      public:
         enum class ScalingMode
         {
            NONE,
            STRETCH,
            LETTERBOX,
            OVERSCAN,
            INTEGER_SCALE
         };

         enum class PresentingMode
         {
            DIRECT,
            ADAPTIVE,
            SINGLE_BUFFERED,
            DOUBLE_BUFFERED,
            TRIPLE_BUFFERED,
            QUADRUPLE_BUFFERED
         };

         FRO_API explicit Renderer();

         Renderer(Renderer const&) = delete;
         Renderer(Renderer&&) = default;

         virtual ~Renderer() override = default;

         Renderer& operator=(Renderer const&) = delete;
         Renderer& operator=(Renderer&&) = default;

         FRO_API void assign_render_target(Window const& window);

         FRO_API Texture const& upload_texture(Surface const& surface);
         FRO_API bool unload_texture(Texture const& texture);

         FRO_API void clear();
         FRO_API void render(Texture const& texture, TransformMatrix const& transform = {},
            SourceRectangle source_rectangle = {});
         FRO_API void render(Shape const& shape, TransformMatrix const& transform = {},
            Color color = { 255, 255, 255, 255 });
         FRO_API void present();

         FRO_API void change_resolution(Vector2<int> resolution);
         FRO_API void change_scaling_mode(ScalingMode scaling_mode);
         FRO_API void change_present_mode(PresentingMode presenting_mode);

         FRO_API [[nodiscard]] SDL_Window& assigned_native_window() const;
         FRO_API [[nodiscard]] Vector2<int> resolution() const;
         FRO_API [[nodiscard]] ScalingMode scaling_mode() const;

         EventDispatcher<SDL_Window> native_window_assigned_event{};

      private:
         [[nodiscard]] static UniquePointer<SDL_Window> create_hidden_native_window();
         [[nodiscard]] static UniquePointer<SDL_Renderer> create_native_renderer(SDL_Window& native_window,
            SDL_Renderer* current_native_renderer = nullptr);

         std::variant<UniquePointer<SDL_Window>, Reference<Window const>> window_;
         UniquePointer<SDL_Renderer> native_renderer_;
         std::vector<Texture> textures_{};
   };
}

#endif
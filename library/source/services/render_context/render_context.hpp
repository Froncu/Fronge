#ifndef RENDER_CONTEXT_HPP
#define RENDER_CONTEXT_HPP

#include "core.hpp"
#include "events/observer/event_dispatcher.hpp"
#include "events/render_context_event.hpp"
#include "froch.hpp"
#include "maths/shapes.hpp"
#include "maths/transform_matrix/transform_matrix.hpp"
#include "maths/vector2.hpp"
#include "reference/reference.hpp"
#include "reference/referenceable.hpp"
#include "services/locator.hpp"
#include "services/system_event_dispatcher/system_event_dispatcher.hpp"
#include "texture.hpp"
#include "utility/unique_pointer.hpp"
#include "utility/variant_visitor.hpp"

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

   class RenderContext final : public Referenceable
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

         FRO_API explicit RenderContext(std::string_view title = "Application", Vector2<int> size = { 640, 480 });

         FRO_API RenderContext(RenderContext const& other);
         RenderContext(RenderContext&&) = default;

         virtual ~RenderContext() override = default;

         FRO_API RenderContext& operator=(RenderContext const& other);
         RenderContext& operator=(RenderContext&&) = default;

         FRO_API Texture const& upload_texture(Surface const& surface);
         FRO_API bool unload_texture(Texture const& texture);

         FRO_API void clear();
         FRO_API void render(Texture const& texture, TransformMatrix const& transform = {},
            SourceRectangle source_rectangle = {});
         FRO_API void render(Shape const& shape, TransformMatrix const& transform = {},
            Color color = { 255, 255, 255, 255 });
         FRO_API void present();

         FRO_API void change_title(std::string_view title);
         FRO_API void change_size(Vector2<int> size);
         FRO_API void change_position(Vector2<int> position);
         FRO_API void center();
         FRO_API void change_fullscreen_mode(bool fullscreen);
         FRO_API void change_resizability(bool resizable);
         FRO_API void change_visibility(bool show);
         FRO_API void change_resolution(Vector2<int> resolution);
         FRO_API void change_scaling_mode(ScalingMode scaling_mode);
         FRO_API void change_present_mode(PresentingMode presenting_mode);

         FRO_API [[nodiscard]] ID::InternalValue id() const;
         FRO_API [[nodiscard]] std::string_view title() const;
         FRO_API [[nodiscard]] Vector2<int> size() const;
         FRO_API [[nodiscard]] Vector2<int> position() const;
         FRO_API [[nodiscard]] bool fullscreen() const;
         FRO_API [[nodiscard]] bool resizable() const;
         FRO_API [[nodiscard]] bool visible() const;
         FRO_API [[nodiscard]] Vector2<int> resolution() const;
         FRO_API [[nodiscard]] ScalingMode scaling_mode() const;

         EventDispatcher<> close_event{};
         EventListener<RenderContextEvent const> on_render_context_event
         {
            VariantVisitor
            {
               [smart_this = Reference{ this }](RenderContextCloseEvent const& event)
               {
                  if (smart_this->id() not_eq event.id)
                     return false;

                  smart_this->close_event.notify();
                  return true;
               },

               [](auto)
               {
                  return false;
               }
            },
            Locator::get<SystemEventDispatcher>().render_context_event
         };

      private:
         RenderContext(std::string_view title, Vector2<int> size,
            std::optional<Vector2<int>> const& position, std::uint64_t flags,
            SDL_Renderer* renderer, std::unordered_set<Texture> const& textures);

         UniquePointer<SDL_Window> native_window_;
         UniquePointer<SDL_Renderer> native_renderer_;
         std::unordered_set<Texture> textures_;
   };
}

#endif
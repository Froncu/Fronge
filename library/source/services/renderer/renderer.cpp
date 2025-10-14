#include <SDL3/SDL.h>

#include "froch.hpp"
#include "renderer.hpp"
#include "services/window/window.hpp"
#include "utility/runtime_assert.hpp"

namespace fro
{
   Renderer::Renderer()
      : native_renderer_{
         []
         {
            SDL_Renderer* const native_renderer{ SDL_CreateRenderer(&Locator::get<Window>().native_window(), nullptr) };
            runtime_assert(native_renderer, std::format("failed to create an SDL renderer ({})", SDL_GetError()));

            return native_renderer;
         }(),
         SDL_DestroyRenderer
      }
   {
   }

   Texture const& Renderer::upload_texture(Surface const& surface)
   {
      return textures_.emplace_back(Texture{ *native_renderer_, surface });
   }

   bool Renderer::unload_texture(Texture const& texture)
   {
      std::erase_if(textures_,
         [&texture](Texture const& stored_texture)
         {
            return &stored_texture == &texture;
         });

      return true;
   }

   void Renderer::clear()
   {
      bool const succeeded{ SDL_RenderClear(native_renderer_.get()) };
      runtime_assert(succeeded, std::format("failed to clear the Renderer ({})", SDL_GetError()));
   }

   void Renderer::render(Texture const& texture, TransformMatrix const& transform, SourceRectangle source_rectangle)
   {
      auto const [texture_width, texture_height]{ texture.size() };

      if (not source_rectangle.width or not source_rectangle.height)
      {
         source_rectangle.x = 0.0;
         source_rectangle.y = 0.0;
         source_rectangle.width = texture_width;
         source_rectangle.height = texture_height;
      }

      SDL_FColor constexpr vertex_color{
         .r{ 1.0f },
         .g{ 1.0f },
         .b{ 1.0f },
         .a{ 1.0f }
      };

      Vector2 const top_left_texture{
         static_cast<float>(source_rectangle.x / texture_width),
         static_cast<float>(source_rectangle.y / texture_height)
      };

      Vector2 const bottom_right_texture{
         static_cast<float>((source_rectangle.x + source_rectangle.width) / texture_width),
         static_cast<float>((source_rectangle.y + source_rectangle.height) / texture_height),
      };

      Vector2 const half_source_size{
         static_cast<float>(source_rectangle.width / 2), static_cast<float>(source_rectangle.height / 2)
      };
      std::array<SDL_Vertex, 4> vertices{
         {
            {
               .position = { -half_source_size.x, -half_source_size.y },
               .color = vertex_color,
               .tex_coord = { top_left_texture.x, top_left_texture.y }
            },
            {
               .position = { half_source_size.x, -half_source_size.y },
               .color = vertex_color,
               .tex_coord = { bottom_right_texture.x, top_left_texture.y }
            },
            {
               .position = { half_source_size.x, half_source_size.y },
               .color = vertex_color,
               .tex_coord = { bottom_right_texture.x, bottom_right_texture.y }
            },
            {
               .position = { -half_source_size.x, half_source_size.y },
               .color = vertex_color,
               .tex_coord = { top_left_texture.x, bottom_right_texture.y }
            }
         }
      };

      for (SDL_Vertex& vertex : vertices)
      {
         Vector3<double> position{ vertex.position.x, vertex.position.y, 1.0 };

         position = transform * position;
         vertex.position.x = static_cast<float>(position.x);
         vertex.position.y = static_cast<float>(position.y);
      }

      std::array constexpr indices
      {
         0, 1, 2,
         2, 3, 0
      };

      bool const succeeded{
         SDL_RenderGeometry(native_renderer_.get(), texture.native_texture_.get(),
            vertices.data(), static_cast<int>(vertices.size()),
            indices.data(), static_cast<int>(indices.size()))
      };
      runtime_assert(succeeded, std::format("failed to render a Texture to the Renderer ({})", SDL_GetError()));
   }

   void Renderer::render(Shape const& shape, TransformMatrix const& transform, Color const color)
   {
      Color old_color;
      bool succeeded{
         SDL_GetRenderDrawColor(native_renderer_.get(), &old_color.red, &old_color.green, &old_color.blue, &old_color.alpha)
      };
      runtime_assert(succeeded, std::format("failed to get the draw color of the Renderer ({})", SDL_GetError()));

      succeeded = SDL_SetRenderDrawColor(native_renderer_.get(), color.red, color.green, color.blue, color.alpha);
      runtime_assert(succeeded, std::format("failed to set the draw color of the Renderer to [{}, {}, {}, {}] ({})", color.red,
         color.green, color.blue, color.alpha, SDL_GetError()));

      std::vector<SDL_FPoint> shape_vertices{};
      VariantVisitor{
         [&transform, &shape_vertices](Circle const& circle)
         {
            std::size_t constexpr vertex_count{ 16 };
            shape_vertices.reserve(vertex_count + 1);

            for (std::size_t index{}; index < vertex_count; ++index)
            {
               double const angle{ index * (2 * std::numbers::pi / (vertex_count - 1)) };

               Vector3 vertex{ std::cos(angle) * circle.radius, std::sin(angle) * circle.radius, 1.0 };
               vertex = transform * vertex;

               shape_vertices.push_back({ static_cast<float>(vertex.x), static_cast<float>(vertex.y) });
            }

            auto const [x, y]{ transform.translation() };
            shape_vertices.push_back({ static_cast<float>(x), static_cast<float>(y) });
         },

         [&transform, &shape_vertices](Polygon const& polygon)
         {
            shape_vertices.reserve(polygon.vertices.size() + 1);
            for (auto const [x, y] : polygon.vertices)
            {
               Vector3 vertex{ x, y, 1.0 };
               vertex = transform * vertex;

               shape_vertices.push_back({ static_cast<float>(vertex.x), static_cast<float>(vertex.y) });
            }

            shape_vertices.emplace_back(shape_vertices.front());
         }
      }(shape);

      succeeded = SDL_RenderLines(native_renderer_.get(), shape_vertices.data(), static_cast<int>(shape_vertices.size()));
      runtime_assert(succeeded, std::format("failed to render a Circle to the Renderer ({})", SDL_GetError()));

      succeeded = SDL_SetRenderDrawColor(native_renderer_.get(), old_color.red, old_color.green, old_color.blue,
         old_color.alpha);
      runtime_assert(succeeded, std::format("failed to set the draw color of the Renderer to [{}, {}, {}, {}] ({})",
         old_color.red, old_color.green, old_color.blue, old_color.alpha, SDL_GetError()));
   }

   void Renderer::present()
   {
      bool const succeeded{ SDL_RenderPresent(native_renderer_.get()) };
      runtime_assert(succeeded, std::format("failed to present the Renderer ({})", SDL_GetError()));
   }

   SDL_RendererLogicalPresentation scaling_mode_to_sdl(Renderer::ScalingMode const scaling_mode)
   {
      switch (scaling_mode)
      {
         case Renderer::ScalingMode::STRETCH:
            return SDL_LOGICAL_PRESENTATION_STRETCH;

         case Renderer::ScalingMode::OVERSCAN:
            return SDL_LOGICAL_PRESENTATION_OVERSCAN;

         case Renderer::ScalingMode::LETTERBOX:
            return SDL_LOGICAL_PRESENTATION_LETTERBOX;

         case Renderer::ScalingMode::INTEGER_SCALE:
            return SDL_LOGICAL_PRESENTATION_INTEGER_SCALE;

         default:
            return SDL_LOGICAL_PRESENTATION_DISABLED;
      }
   }

   void Renderer::change_resolution(Vector2<int> const resolution)
   {
      ScalingMode const current_scaling_mode{ scaling_mode() };
      if (current_scaling_mode == ScalingMode::NONE)
         Locator::get<Logger>().warning("Renderer's scaling mode is NONE; "
            "changing the resolution will not have any effect until the scaling mode is changed");

      bool const succeeded{
         SDL_SetRenderLogicalPresentation(native_renderer_.get(), resolution.x, resolution.y,
            scaling_mode_to_sdl(current_scaling_mode))
      };
      runtime_assert(succeeded, std::format("failed to change the Renderer's resolution ({})", SDL_GetError()));
   }

   void Renderer::change_scaling_mode(ScalingMode const scaling_mode)
   {
      auto const [resolution_width, resolution_height]{ resolution() };
      bool const succeeded{
         SDL_SetRenderLogicalPresentation(native_renderer_.get(), resolution_width, resolution_height,
            scaling_mode_to_sdl(scaling_mode))
      };
      runtime_assert(succeeded, std::format("failed to change the Renderer's scaling mode ({})", SDL_GetError()));
   }

   void Renderer::change_present_mode(PresentingMode presenting_mode)
   {
      int native_presenting_mode;

      switch (presenting_mode)
      {
         case PresentingMode::ADAPTIVE:
            native_presenting_mode = SDL_RENDERER_VSYNC_ADAPTIVE;
            break;

         case PresentingMode::SINGLE_BUFFERED:
            native_presenting_mode = 1;
            break;

         case PresentingMode::DOUBLE_BUFFERED:
            native_presenting_mode = 2;
            break;

         case PresentingMode::TRIPLE_BUFFERED:
            native_presenting_mode = 3;
            break;

         case PresentingMode::QUADRUPLE_BUFFERED:
            native_presenting_mode = 4;
            break;

         default:
            native_presenting_mode = SDL_RENDERER_VSYNC_DISABLED;
            break;
      }

      if (not SDL_SetRenderVSync(native_renderer_.get(), native_presenting_mode))
         Locator::get<Logger>().warning(std::format("failed to change the Renderer's presenting mode to {} ({})",
            static_cast<int>(presenting_mode), SDL_GetError()));
   }

   Vector2<int> Renderer::resolution() const
   {
      Vector2<int> resolution;
      bool const succeeded{ SDL_GetRenderLogicalPresentation(native_renderer_.get(), &resolution.x, &resolution.y, nullptr) };
      runtime_assert(succeeded, std::format("failed to retrieve the Renderer's resolution ({})", SDL_GetError()));

      return resolution;
   }

   Renderer::ScalingMode Renderer::scaling_mode() const
   {
      SDL_RendererLogicalPresentation native_scaling_mode;
      bool const succeeded{ SDL_GetRenderLogicalPresentation(native_renderer_.get(), nullptr, nullptr, &native_scaling_mode) };
      runtime_assert(succeeded, std::format("failed to retrieve the Renderer's scaling mode ({})", SDL_GetError()));

      switch (native_scaling_mode)
      {
         case SDL_LOGICAL_PRESENTATION_STRETCH:
            return ScalingMode::STRETCH;

         case SDL_LOGICAL_PRESENTATION_LETTERBOX:
            return ScalingMode::LETTERBOX;

         case SDL_LOGICAL_PRESENTATION_OVERSCAN:
            return ScalingMode::OVERSCAN;

         case SDL_LOGICAL_PRESENTATION_INTEGER_SCALE:
            return ScalingMode::INTEGER_SCALE;

         default:
            return ScalingMode::NONE;
      }
   }
}
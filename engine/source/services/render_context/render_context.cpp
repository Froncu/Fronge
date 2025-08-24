#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>

#include "froch.hpp"
#include "render_context.hpp"
#include "utility/runtime_assert.hpp"

namespace fro
{
   RenderContext::RenderContext(std::string_view const title, Vector2<int> const size,
      std::optional<Vector2<int>> const& position, std::uint64_t const flags,
      SDL_Renderer* renderer, std::unordered_set<Texture> const& textures)
      : native_window_{
         [](std::string_view const title, Vector2<int> const size, std::uint64_t const flags)
         {
            SDL_Window* const native_window{ SDL_CreateWindow(title.data(), size.x, size.y, flags) };
            runtime_assert(native_window, "failed to create an SDL window ({})",
               SDL_GetError());
            return native_window;
         }(title, size, flags),
         SDL_DestroyWindow
      }
      , native_renderer_{
         [](SDL_Window* const native_window)
         {
            SDL_Renderer* const native_renderer{ SDL_CreateRenderer(native_window, nullptr) };
            runtime_assert(native_renderer, "failed to create an SDL renderer ({})",
               SDL_GetError());
            return native_renderer;
         }(native_window_.get()),
         SDL_DestroyRenderer
      }
      , textures_{
         [](SDL_Renderer& native_renderer, std::unordered_set<Texture> const& textures)
         {
            std::unordered_set<Texture> copied_textures{};
            copied_textures.reserve(textures.size());

            for (Texture const& texture : textures)
               copied_textures.insert(Texture{ native_renderer, texture });

            return copied_textures;
         }(*native_renderer_, textures)
      }
   {
      if (position.has_value())
         change_position(*position);

      if (renderer)
      {
         int width;
         int height;
         SDL_RendererLogicalPresentation scaling_mode;
         SDL_GetRenderLogicalPresentation(renderer, &width, &height, &scaling_mode);
         SDL_SetRenderLogicalPresentation(native_renderer_.get(), width, height, scaling_mode);

         int vsync;
         SDL_GetRenderVSync(native_renderer_.get(), &vsync);
         SDL_SetRenderVSync(native_renderer_.get(), vsync);
      }

      bool succeeded{ ImGui_ImplSDLRenderer3_Init(native_renderer_.get()) };
      runtime_assert(succeeded, "failed to initialize ImGui's SDL implementation");

      succeeded = ImGui_ImplSDL3_InitForSDLRenderer(native_window_.get(), native_renderer_.get());
      runtime_assert(succeeded, "failed to initialize ImGui for SDL renderer");
   }

   RenderContext::RenderContext(std::string_view const title, Vector2<int> const size)
      : RenderContext(title, size, {}, {}, {}, {})
   {
   }

   RenderContext::RenderContext(RenderContext const& other)
      : RenderContext(other.title(), other.size(),
         other.position(), SDL_GetWindowFlags(other.native_window_.get()),
         other.native_renderer_.get(), other.textures_)
   {
   }

   RenderContext::~RenderContext()
   {
      ImGui_ImplSDL3_Shutdown();
      ImGui_ImplSDLRenderer3_Shutdown();
   }

   RenderContext& RenderContext::operator=(RenderContext const& other)
   {
      if (this == &other)
         return *this;

      return *this = RenderContext{ other };
   }

   Texture const& RenderContext::upload_texture(Surface const& surface)
   {
      // NOTE: you could check if the texture is created first to avoid unneccesary creation of it
      // if it already exists, but this method should not be called often in a hot path so it's fine
      return *textures_.insert(Texture{ *native_renderer_, surface }).first;
   }

   bool RenderContext::unload_texture(Texture const& texture)
   {
      auto const stored_texture{ textures_.find(texture) };
      if (stored_texture == textures_.end())
         return false;

      textures_.erase(stored_texture);
      return true;
   }

   void RenderContext::begin()
   {
      bool const succeeded{ SDL_RenderClear(native_renderer_.get()) };
      runtime_assert(succeeded, "failed to clear the RenderContext{} ({})",
         id(), SDL_GetError());

      ImGui_ImplSDLRenderer3_NewFrame();
      ImGui_ImplSDL3_NewFrame();
      ImGui::NewFrame();
   }

   void RenderContext::render(Texture const& texture, TransformMatrix const& transform, SourceRectangle source_rectangle)
   {
      auto const stored_texture{ textures_.find(texture) };

      if (stored_texture == textures_.end())
         exception("attempted to render a Texture which was not uploaded to RenderContext{}",
            id());

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
         SDL_RenderGeometry(native_renderer_.get(), stored_texture->native_texture_.get(),
            vertices.data(), static_cast<int>(vertices.size()),
            indices.data(), static_cast<int>(indices.size()))
      };
      runtime_assert(succeeded, "failed to render a Texture to RenderContext{} ({})",
         id(), SDL_GetError());
   }

   void RenderContext::render(Shape const& shape, TransformMatrix const& transform, Color const color)
   {
      Color old_color;
      bool succeeded{
         SDL_GetRenderDrawColor(native_renderer_.get(), &old_color.red, &old_color.green, &old_color.blue, &old_color.alpha)
      };
      runtime_assert(succeeded, "failed to get the draw color of RenderContext{} ({})",
         id(), SDL_GetError());

      succeeded = SDL_SetRenderDrawColor(native_renderer_.get(), color.red, color.green, color.blue, color.alpha);
      runtime_assert(succeeded, "failed to set the draw color of RenderContext{} to [{}, {}, {}, {}] ({})",
         id(), color.red, color.green, color.blue, color.alpha, SDL_GetError());

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
      runtime_assert(succeeded, "failed to render a Circle to RenderContext{} ({})",
         id(), SDL_GetError());

      succeeded = SDL_SetRenderDrawColor(native_renderer_.get(), old_color.red, old_color.green, old_color.blue,
         old_color.alpha);
      runtime_assert(succeeded, "failed to set the draw color of RenderContext{} to [{}, {}, {}, {}] ({})",
         id(), old_color.red, old_color.green, old_color.blue, old_color.alpha, SDL_GetError());
   }

   void RenderContext::present()
   {
      ScalingMode const current_scaling_mode{ scaling_mode() };
      change_scaling_mode(ScalingMode::NONE);
      ImGui::Render();
      ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), native_renderer_.get());
      change_scaling_mode(current_scaling_mode);

      if ((ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) == ImGuiConfigFlags_ViewportsEnable)
      {
         ImGui::UpdatePlatformWindows();
         ImGui::RenderPlatformWindowsDefault();
      }

      bool const succeeded{ SDL_RenderPresent(native_renderer_.get()) };
      runtime_assert(succeeded, "failed to present the RenderContext{} ({})",
         id(), SDL_GetError());
   }

   void RenderContext::change_title(std::string_view const title)
   {
      bool const succeeded{ SDL_SetWindowTitle(native_window_.get(), title.data()) };
      runtime_assert(succeeded, "failed to set RenderContext{}'s title to {} ({})",
         id(), title, SDL_GetError());
   }

   void RenderContext::change_size(Vector2<int> const size)
   {
      bool const succeeded{ SDL_SetWindowSize(native_window_.get(), size.x, size.y) };
      runtime_assert(succeeded, "failed to set RenderContext{}'s size to {}x{} ({})",
         id(), size.x, size.y, SDL_GetError());
   }

   void RenderContext::change_position(Vector2<int> position)
   {
      bool const succeeded{ SDL_SetWindowPosition(native_window_.get(), position.x, position.y) };
      runtime_assert(succeeded, "failed to set RenderContext{}'s position to {}x{} ({})",
         id(), position.x, position.y, SDL_GetError());
   }

   void RenderContext::center()
   {
      change_position({ SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED });
   }

   void RenderContext::change_fullscreen_mode(bool const fullscreen)
   {
      bool const succeeded{ SDL_SetWindowFullscreen(native_window_.get(), fullscreen) };
      runtime_assert(succeeded, "failed to set RenderContext{}'s fullscreen to {} ({})",
         id(), fullscreen ? "fullscreen" : "windowed", SDL_GetError());
   }

   void RenderContext::change_resizability(bool const resizable)
   {
      bool const succeeded{ SDL_SetWindowResizable(native_window_.get(), resizable) };
      runtime_assert(succeeded, "failed to set RenderContext{}'s resizability to {} ({})",
         id(), resizable, SDL_GetError());
   }

   void RenderContext::change_visibility(bool const show)
   {
      bool const succeeded{ show ? SDL_ShowWindow(native_window_.get()) : SDL_HideWindow(native_window_.get()) };
      runtime_assert(succeeded, "failed to set RenderContext{}'s visibility to {} ({})",
         id(), show ? "show" : "hidden", SDL_GetError());
   }

   SDL_RendererLogicalPresentation scaling_mode_to_sdl(RenderContext::ScalingMode const scaling_mode)
   {
      switch (scaling_mode)
      {
         case RenderContext::ScalingMode::STRETCH:
            return SDL_LOGICAL_PRESENTATION_STRETCH;

         case RenderContext::ScalingMode::OVERSCAN:
            return SDL_LOGICAL_PRESENTATION_OVERSCAN;

         case RenderContext::ScalingMode::LETTERBOX:
            return SDL_LOGICAL_PRESENTATION_LETTERBOX;

         case RenderContext::ScalingMode::INTEGER_SCALE:
            return SDL_LOGICAL_PRESENTATION_INTEGER_SCALE;

         default:
            return SDL_LOGICAL_PRESENTATION_DISABLED;
      }
   }

   void RenderContext::change_resolution(Vector2<int> const resolution)
   {
      ScalingMode const current_scaling_mode{ scaling_mode() };
      if (current_scaling_mode == ScalingMode::NONE)
         Locator::get<Logger>().warning("RenderContext{}'s scaling mode is NONE; "
            "changing the resolution will not have any effect until the scaling mode is changed",
            id());

      bool const succeeded{
         SDL_SetRenderLogicalPresentation(native_renderer_.get(), resolution.x, resolution.y,
            scaling_mode_to_sdl(current_scaling_mode))
      };
      runtime_assert(succeeded, "failed to change RenderContext{}'s scaling mode ({})",
         id(), SDL_GetError());
   }

   void RenderContext::change_scaling_mode(ScalingMode const scaling_mode)
   {
      auto const [resolution_width, resolution_height]{ resolution() };
      bool const succeeded{
         SDL_SetRenderLogicalPresentation(native_renderer_.get(), resolution_width, resolution_height,
            scaling_mode_to_sdl(scaling_mode))
      };
      runtime_assert(succeeded, "failed to change RenderContext{}'s scaling mode ({})",
         id(), SDL_GetError());
   }

   void RenderContext::change_present_mode(PresentingMode presenting_mode)
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
         Locator::get<Logger>().warning("failed to change RenderContext{}'s presenting mode to {} ({})",
            id(), static_cast<int>(presenting_mode), SDL_GetError());
   }

   ID::InternalValue RenderContext::id() const
   {
      ID::InternalValue const id{ SDL_GetWindowID(native_window_.get()) };
      runtime_assert(id, "failed to retrieve the ID of a RenderContext ({})",
         SDL_GetError());

      return id;
   }

   std::string_view RenderContext::title() const
   {
      return SDL_GetWindowTitle(native_window_.get());
   }

   Vector2<int> RenderContext::size() const
   {
      Vector2<int> size;
      bool const succeeded{ SDL_GetWindowSize(native_window_.get(), &size.x, &size.y) };
      runtime_assert(succeeded, "failed to retrieve RenderContext{}'s size ({})",
         id(), SDL_GetError());

      return size;
   }

   Vector2<int> RenderContext::position() const
   {
      Vector2<int> position;
      bool const succeeded{ SDL_GetWindowPosition(native_window_.get(), &position.x, &position.y) };
      runtime_assert(succeeded, "failed to retrieve RenderContext{}'s position ({})",
         id(), SDL_GetError());

      return position;
   }

   bool RenderContext::fullscreen() const
   {
      return SDL_GetWindowFlags(native_window_.get()) & SDL_WINDOW_FULLSCREEN;
   }

   bool RenderContext::resizable() const
   {
      return SDL_GetWindowFlags(native_window_.get()) & SDL_WINDOW_RESIZABLE;
   }

   bool RenderContext::visible() const
   {
      return not(SDL_GetWindowFlags(native_window_.get()) & SDL_WINDOW_HIDDEN);
   }

   Vector2<int> RenderContext::resolution() const
   {
      Vector2<int> resolution;
      bool const succeeded{ SDL_GetRenderLogicalPresentation(native_renderer_.get(), &resolution.x, &resolution.y, nullptr) };
      runtime_assert(succeeded, "failed to retrieve RenderContext{}'s resolution ({})",
         id(), SDL_GetError());

      return resolution;
   }

   RenderContext::ScalingMode RenderContext::scaling_mode() const
   {
      SDL_RendererLogicalPresentation native_scaling_mode;
      bool const succeeded{ SDL_GetRenderLogicalPresentation(native_renderer_.get(), nullptr, nullptr, &native_scaling_mode) };
      runtime_assert(succeeded, "failed to retrieve RenderContext{}'s scaling mode ({})",
         id(), SDL_GetError());

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
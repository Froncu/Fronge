#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "core.hpp"
#include "froch.hpp"
#include "reference/referenceable.hpp"
#include "utility/unique_pointer.hpp"

struct SDL_GPUDevice;
struct SDL_GPUCommandBuffer;
struct SDL_GPURenderPass;

namespace fro
{
   class EditorUI;

   class Renderer final : public Referenceable
   {
      friend EditorUI;

      public:
         FRO_API explicit Renderer();

         Renderer(Renderer const&) = delete;
         Renderer(Renderer&&) = default;

         virtual ~Renderer() override;

         Renderer& operator=(Renderer const&) = delete;
         Renderer& operator=(Renderer&&) = default;

         FRO_API void acquire_command_buffer();
         FRO_API void start_render_pass();
         FRO_API void end_render_pass();
         FRO_API void submit();

         FRO_API [[nodiscard]] SDL_GPUDevice& gpu_device() const;
         FRO_API [[nodiscard]] SDL_GPUCommandBuffer* command_buffer() const;
         FRO_API [[nodiscard]] SDL_GPURenderPass* render_pass() const;

      private:
         UniquePointer<SDL_GPUDevice> gpu_device_;
         SDL_GPUCommandBuffer* command_buffer_{};
         SDL_GPURenderPass* render_pass_{};
   };
}

#endif
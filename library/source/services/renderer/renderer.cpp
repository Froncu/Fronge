#include <SDL3/SDL.h>

#include "froch.hpp"
#include "renderer.hpp"
#include "services/window/window.hpp"
#include "utility/runtime_assert.hpp"

namespace fro
{
   Renderer::Renderer()
      : gpu_device_{
         []
         {
            SDL_GPUDevice* const gpu_device{ SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, DEBUG, nullptr) };
            runtime_assert(gpu_device, "failed to create an SDL GPU device ({})",
               SDL_GetError());

            return gpu_device;
         }(),
         SDL_DestroyGPUDevice
      }
   {
      bool const succeeded{ SDL_ClaimWindowForGPUDevice(gpu_device_.get(), &Locator::get<Window>().native_window()) };
      runtime_assert(succeeded, "failed to claim the window for GPU device ({})",
         SDL_GetError());
   }

   Renderer::~Renderer()
   {
      SDL_ReleaseWindowFromGPUDevice(gpu_device_.get(), &Locator::get<Window>().native_window());
   }

   void Renderer::acquire_command_buffer()
   {
      command_buffer_ = SDL_AcquireGPUCommandBuffer(gpu_device_.get());
      runtime_assert(command_buffer_, "failed to acquire GPU command buffer ({})",
         SDL_GetError());
   }

   void Renderer::start_render_pass()
   {
      SDL_GPUTexture* swapchain_texture;
      bool succeeded{
         SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer_, &Locator::get<Window>().native_window(), &swapchain_texture,
            nullptr, nullptr)
      };
      runtime_assert(succeeded, "failed to acquire GPU swapchain texture ({})",
         SDL_GetError());

      if (not swapchain_texture)
      {
         succeeded = SDL_CancelGPUCommandBuffer(command_buffer_);
         runtime_assert(succeeded, "failed to cancel GPU command buffer ({})",
            SDL_GetError());

         command_buffer_ = nullptr;
         return;
      }

      SDL_GPUColorTargetInfo const target_info{
         .texture{ swapchain_texture },
         .load_op{ SDL_GPU_LOADOP_CLEAR },
         .store_op{ SDL_GPU_STOREOP_STORE },
      };

      render_pass_ = SDL_BeginGPURenderPass(command_buffer_, &target_info, 1, nullptr);
      runtime_assert(render_pass_, "failed to begin GPU render pass ({})",
         SDL_GetError());
   }

   void Renderer::end_render_pass()
   {
      SDL_EndGPURenderPass(render_pass_);
      render_pass_ = nullptr;
   }

   void Renderer::submit()
   {
      bool const succeeded{ SDL_SubmitGPUCommandBuffer(command_buffer_) };
      runtime_assert(succeeded, "failed to submit GPU command buffer ({})",
         SDL_GetError());

      command_buffer_ = nullptr;
   }

   SDL_GPUDevice& Renderer::gpu_device() const
   {
      return *gpu_device_;
   }

   SDL_GPUCommandBuffer* Renderer::command_buffer() const
   {
      return command_buffer_;
   }

   SDL_GPURenderPass* Renderer::render_pass() const
   {
      return render_pass_;
   }
}
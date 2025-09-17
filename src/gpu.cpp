#include <napi.h>
#include <SDL3/SDL_gpu.h>

using namespace Napi;

Value Wrap_SDL_GPUSupportsShaderFormats(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 2 || !info[0].IsNumber() || !info[1].IsString()) {
    TypeError::New(env, "Expected number, string").ThrowAsJavaScriptException();
    return env.Undefined();
  }
  SDL_GPUShaderFormat format_flags = static_cast<SDL_GPUShaderFormat>(info[0].As<Number>().Uint32Value());
  std::string name = info[1].As<String>().Utf8Value();
  return Boolean::New(env, SDL_GPUSupportsShaderFormats(format_flags, name.c_str()));
}

// Similar wrappers for other GPU functions like SDL_GPUSupportsProperties, SDL_CreateGPUDevice, SDL_CreateGPUDeviceWithProperties, SDL_DestroyGPUDevice, SDL_GetNumGPUDrivers, SDL_GetGPUDriver, SDL_GetGPUDeviceDriver, SDL_GetGPUShaderFormats, SDL_GetGPUDeviceProperties, SDL_CreateGPUComputePipeline, SDL_CreateGPUGraphicsPipeline, SDL_CreateGPUSampler, SDL_CreateGPUShader, SDL_CreateGPUTexture, SDL_CreateGPUBuffer, SDL_CreateGPUTransferBuffer, SDL_SetGPUBufferName, SDL_SetGPUTextureName, SDL_InsertGPUDebugLabel, SDL_PushGPUDebugGroup, SDL_PopGPUDebugGroup, SDL_ReleaseGPUTexture, SDL_ReleaseGPUSampler, SDL_ReleaseGPUBuffer, SDL_ReleaseGPUTransferBuffer, SDL_ReleaseGPUComputePipeline, SDL_ReleaseGPUShader, SDL_ReleaseGPUGraphicsPipeline, SDL_AcquireGPUCommandBuffer, SDL_PushGPUVertexUniformData, SDL_PushGPUFragmentUniformData, SDL_PushGPUComputeUniformData, SDL_BeginGPURenderPass, SDL_BindGPUGraphicsPipeline, SDL_SetGPUViewport, SDL_SetGPUScissor, SDL_SetGPUBlendConstants, SDL_SetGPUStencilReference, SDL_BindGPUVertexBuffers, SDL_BindGPUIndexBuffer, SDL_BindGPUVertexSamplers

void InitGPU(Env env, Object exports) {
  exports.Set("GPUSupportsShaderFormats", Function::New(env, Wrap_SDL_GPUSupportsShaderFormats));
  // Add all other GPU wrappers here...
}

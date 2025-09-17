#include <napi.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_assert.h>
#include <SDL3/SDL_version.h>

using namespace Napi;

extern void InitAudio(Env env, Object exports);
extern void InitGPU(Env env, Object exports);
extern void InitVideo(Env env, Object exports);
extern void InitInput(Env env, Object exports);

Value Wrap_SDL_Init(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected a number for flags").ThrowAsJavaScriptException();
    return env.Undefined();
  }
  Uint32 flags = info[0].As<Number>().Uint32Value();
  bool result = SDL_Init(flags);
  if (!result) {
    Error::New(env, SDL_GetError()).ThrowAsJavaScriptException();
    return env.Undefined();
  }
  return Boolean::New(env, result);
}

Value Wrap_SDL_Quit(const CallbackInfo& info) {
  Env env = info.Env();
  SDL_Quit();
  return env.Undefined();
}

// Similar wrappers for other basics functions like SDL_InitSubSystem, SDL_QuitSubSystem, SDL_WasInit, SDL_IsMainThread, SDL_RunOnMainThread, SDL_SetAppMetadata, SDL_SetAppMetadataProperty, SDL_GetAppMetadataProperty

Value Wrap_SDL_SetHintWithPriority(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 3 || !info[0].IsString() || !info[1].IsString() || !info[2].IsNumber()) {
    TypeError::New(env, "Expected string, string, number").ThrowAsJavaScriptException();
    return env.Undefined();
  }
  std::string name = info[0].As<String>().Utf8Value();
  std::string value = info[1].As<String>().Utf8Value();
  SDL_HintPriority priority = static_cast<SDL_HintPriority>(info[2].As<Number>().Int32Value());
  return Boolean::New(env, SDL_SetHintWithPriority(name.c_str(), value.c_str(), priority));
}

// Similar wrappers for other hints, properties, error, log, assert, version functions based on the signatures.

Object Init (Env env, Object exports) {
  exports.Set("Init", Function::New(env, Wrap_SDL_Init));
  exports.Set("Quit", Function::New(env, Wrap_SDL_Quit));
  exports.Set("SetHintWithPriority", Function::New(env, Wrap_SDL_SetHintWithPriority));
  // Add all other basics wrappers here...

  InitAudio(env, exports);
  InitGPU(env, exports);
  InitVideo(env, exports);
  InitInput(env, exports);

  return exports;
}

NODE_API_MODULE(sdl, Init)

#include <napi.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_assert.h>
#include <SDL3/SDL_version.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_audio.h>

using namespace Napi;

// Commenting out extern declarations to avoid linking issues
// extern void InitAudio(Env env, Object exports);
// extern void InitGPU(Env env, Object exports);
// extern void InitVideo(Env env, Object exports);
// extern void InitInput(Env env, Object exports);

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

Value Wrap_SDL_GetError(const CallbackInfo& info) {
  Env env = info.Env();
  const char* error = SDL_GetError();
  if (error) {
    return String::New(env, error);
  }
  return String::New(env, "");
}

Value Wrap_SDL_Delay(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected a number for delay").ThrowAsJavaScriptException();
    return env.Undefined();
  }
  Uint32 ms = info[0].As<Number>().Uint32Value();
  SDL_Delay(ms);
  return env.Undefined();
}

// Video functions implemented directly here
Value Wrap_SDL_CreateWindow(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 6 || !info[0].IsString() || !info[1].IsNumber() || !info[2].IsNumber() ||
      !info[3].IsNumber() || !info[4].IsNumber() || !info[5].IsNumber()) {
    TypeError::New(env, "Expected string, number, number, number, number, number").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  std::string title = info[0].As<String>().Utf8Value();
  int w = info[3].As<Number>().Int32Value();
  int h = info[4].As<Number>().Int32Value();
  Uint64 flags = info[5].As<Number>().Int64Value();

  SDL_Window* window = SDL_CreateWindow(title.c_str(), w, h, flags);
  if (!window) {
    return env.Null();
  }

  return Number::New(env, reinterpret_cast<uintptr_t>(window));
}

Value Wrap_SDL_DestroyWindow(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected a number (window pointer)").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_Window* window = reinterpret_cast<SDL_Window*>(info[0].As<Number>().Int64Value());
  SDL_DestroyWindow(window);
  return env.Undefined();
}

Value Wrap_SDL_CreateRenderer(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() < 2 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected window pointer and optional name").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_Window* window = reinterpret_cast<SDL_Window*>(info[0].As<Number>().Int64Value());
  const char* name = nullptr;

  SDL_Renderer* renderer = SDL_CreateRenderer(window, name);
  if (!renderer) {
    return env.Null();
  }

  return Number::New(env, reinterpret_cast<uintptr_t>(renderer));
}

Value Wrap_SDL_DestroyRenderer(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected a number (renderer pointer)").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_Renderer* renderer = reinterpret_cast<SDL_Renderer*>(info[0].As<Number>().Int64Value());
  SDL_DestroyRenderer(renderer);
  return env.Undefined();
}

Value Wrap_SDL_SetRenderDrawColor(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 5 || !info[0].IsNumber() || !info[1].IsNumber() ||
      !info[2].IsNumber() || !info[3].IsNumber() || !info[4].IsNumber()) {
    TypeError::New(env, "Expected renderer, r, g, b, a").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_Renderer* renderer = reinterpret_cast<SDL_Renderer*>(info[0].As<Number>().Int64Value());
  Uint8 r = info[1].As<Number>().Uint32Value();
  Uint8 g = info[2].As<Number>().Uint32Value();
  Uint8 b = info[3].As<Number>().Uint32Value();
  Uint8 a = info[4].As<Number>().Uint32Value();

  bool result = SDL_SetRenderDrawColor(renderer, r, g, b, a);
  return Boolean::New(env, result);
}

Value Wrap_SDL_RenderClear(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected renderer").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_Renderer* renderer = reinterpret_cast<SDL_Renderer*>(info[0].As<Number>().Int64Value());
  bool result = SDL_RenderClear(renderer);
  return Boolean::New(env, result);
}

Value Wrap_SDL_RenderRect(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 2 || !info[0].IsNumber() || !info[1].IsObject()) {
    TypeError::New(env, "Expected renderer, rect object").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_Renderer* renderer = reinterpret_cast<SDL_Renderer*>(info[0].As<Number>().Int64Value());
  Object rectObj = info[1].As<Object>();

  SDL_FRect rect;
  rect.x = rectObj.Get("x").As<Number>().FloatValue();
  rect.y = rectObj.Get("y").As<Number>().FloatValue();
  rect.w = rectObj.Get("w").As<Number>().FloatValue();
  rect.h = rectObj.Get("h").As<Number>().FloatValue();

  bool result = SDL_RenderRect(renderer, &rect);
  return Boolean::New(env, result);
}

Value Wrap_SDL_RenderPresent(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected renderer").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_Renderer* renderer = reinterpret_cast<SDL_Renderer*>(info[0].As<Number>().Int64Value());
  bool result = SDL_RenderPresent(renderer);
  return Boolean::New(env, result);
}

Value Wrap_SDL_PollEvent(const CallbackInfo& info) {
  Env env = info.Env();
  SDL_Event event;
  bool result = SDL_PollEvent(&event);
  if (result) {
    Object jsEvent = Object::New(env);
    jsEvent.Set("type", Number::New(env, event.type));
    return jsEvent;
  }
  return env.Null();
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

// Audio Device Functions
Value Wrap_SDL_OpenAudioDevice(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() < 1 || !info[0].IsObject()) {
    TypeError::New(env, "Expected audio spec object").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  Object specObj = info[0].As<Object>();
  SDL_AudioSpec spec;
  spec.format = static_cast<SDL_AudioFormat>(specObj.Get("format").As<Number>().Uint32Value());
  spec.channels = specObj.Get("channels").As<Number>().Int32Value();
  spec.freq = specObj.Get("freq").As<Number>().Int32Value();

  SDL_AudioDeviceID devid = info.Length() > 1 ? info[1].As<Number>().Uint32Value() : SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK;

  SDL_AudioDeviceID device = SDL_OpenAudioDevice(devid, &spec);
  if (device == 0) {
    return env.Null();
  }

  return Number::New(env, device);
}

Value Wrap_SDL_CloseAudioDevice(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected device ID").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_AudioDeviceID devid = info[0].As<Number>().Uint32Value();
  SDL_CloseAudioDevice(devid);
  return env.Undefined();
}

Value Wrap_SDL_GetAudioDeviceName(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected device ID").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_AudioDeviceID devid = info[0].As<Number>().Uint32Value();
  const char* name = SDL_GetAudioDeviceName(devid);
  if (!name) {
    return env.Null();
  }
  return String::New(env, name);
}

Value Wrap_SDL_PauseAudioDevice(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected device ID").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_AudioDeviceID devid = info[0].As<Number>().Uint32Value();
  bool result = SDL_PauseAudioDevice(devid);
  return Boolean::New(env, result);
}

Value Wrap_SDL_ResumeAudioDevice(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected device ID").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_AudioDeviceID devid = info[0].As<Number>().Uint32Value();
  bool result = SDL_ResumeAudioDevice(devid);
  return Boolean::New(env, result);
}

Value Wrap_SDL_AudioDevicePaused(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected device ID").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_AudioDeviceID devid = info[0].As<Number>().Uint32Value();
  bool result = SDL_AudioDevicePaused(devid);
  return Boolean::New(env, result);
}

Value Wrap_SDL_SetAudioDeviceGain(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
    TypeError::New(env, "Expected device ID and gain").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_AudioDeviceID devid = info[0].As<Number>().Uint32Value();
  float gain = info[1].As<Number>().FloatValue();
  bool result = SDL_SetAudioDeviceGain(devid, gain);
  return Boolean::New(env, result);
}

Value Wrap_SDL_GetAudioDeviceGain(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected device ID").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_AudioDeviceID devid = info[0].As<Number>().Uint32Value();
  float gain = SDL_GetAudioDeviceGain(devid);
  return Number::New(env, gain);
}

// Audio Stream Functions
Value Wrap_SDL_CreateAudioStream(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 2 || !info[0].IsObject() || !info[1].IsObject()) {
    TypeError::New(env, "Expected src and dst audio spec objects").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  Object srcSpecObj = info[0].As<Object>();
  Object dstSpecObj = info[1].As<Object>();

  SDL_AudioSpec srcSpec, dstSpec;
  srcSpec.format = static_cast<SDL_AudioFormat>(srcSpecObj.Get("format").As<Number>().Uint32Value());
  srcSpec.channels = srcSpecObj.Get("channels").As<Number>().Int32Value();
  srcSpec.freq = srcSpecObj.Get("freq").As<Number>().Int32Value();

  dstSpec.format = static_cast<SDL_AudioFormat>(dstSpecObj.Get("format").As<Number>().Uint32Value());
  dstSpec.channels = dstSpecObj.Get("channels").As<Number>().Int32Value();
  dstSpec.freq = dstSpecObj.Get("freq").As<Number>().Int32Value();

  SDL_AudioStream* stream = SDL_CreateAudioStream(&srcSpec, &dstSpec);
  if (!stream) {
    return env.Null();
  }

  return Number::New(env, reinterpret_cast<uintptr_t>(stream));
}

Value Wrap_SDL_DestroyAudioStream(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected stream pointer").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_AudioStream* stream = reinterpret_cast<SDL_AudioStream*>(info[0].As<Number>().Int64Value());
  SDL_DestroyAudioStream(stream);
  return env.Undefined();
}

Value Wrap_SDL_BindAudioStream(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
    TypeError::New(env, "Expected device ID and stream pointer").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_AudioDeviceID devid = info[0].As<Number>().Uint32Value();
  SDL_AudioStream* stream = reinterpret_cast<SDL_AudioStream*>(info[1].As<Number>().Int64Value());

  bool result = SDL_BindAudioStream(devid, stream);
  return Boolean::New(env, result);
}

Value Wrap_SDL_UnbindAudioStream(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected stream pointer").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_AudioStream* stream = reinterpret_cast<SDL_AudioStream*>(info[0].As<Number>().Int64Value());
  SDL_UnbindAudioStream(stream);
  return env.Undefined();
}

Value Wrap_SDL_PutAudioStreamData(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 2 || !info[0].IsNumber() || !info[1].IsArrayBuffer()) {
    TypeError::New(env, "Expected stream pointer and ArrayBuffer").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_AudioStream* stream = reinterpret_cast<SDL_AudioStream*>(info[0].As<Number>().Int64Value());
  ArrayBuffer buffer = info[1].As<ArrayBuffer>();

  bool result = SDL_PutAudioStreamData(stream, buffer.Data(), buffer.ByteLength());
  return Boolean::New(env, result);
}

Value Wrap_SDL_GetAudioStreamData(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 2 || !info[0].IsNumber() || !info[1].IsNumber()) {
    TypeError::New(env, "Expected stream pointer and buffer size").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_AudioStream* stream = reinterpret_cast<SDL_AudioStream*>(info[0].As<Number>().Int64Value());
  int len = info[1].As<Number>().Int32Value();

  ArrayBuffer buffer = ArrayBuffer::New(env, len);
  int result = SDL_GetAudioStreamData(stream, buffer.Data(), len);

  if (result < 0) {
    return env.Null();
  }

  // Return both the buffer and actual bytes read
  Object returnObj = Object::New(env);
  returnObj.Set("buffer", buffer);
  returnObj.Set("bytesRead", Number::New(env, result));
  return returnObj;
}

Value Wrap_SDL_GetAudioStreamAvailable(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected stream pointer").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_AudioStream* stream = reinterpret_cast<SDL_AudioStream*>(info[0].As<Number>().Int64Value());
  int available = SDL_GetAudioStreamAvailable(stream);
  return Number::New(env, available);
}

Value Wrap_SDL_FlushAudioStream(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected stream pointer").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_AudioStream* stream = reinterpret_cast<SDL_AudioStream*>(info[0].As<Number>().Int64Value());
  bool result = SDL_FlushAudioStream(stream);
  return Boolean::New(env, result);
}

Value Wrap_SDL_ClearAudioStream(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected stream pointer").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  SDL_AudioStream* stream = reinterpret_cast<SDL_AudioStream*>(info[0].As<Number>().Int64Value());
  bool result = SDL_ClearAudioStream(stream);
  return Boolean::New(env, result);
}

Value Wrap_SDL_LoadWAV(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsString()) {
    TypeError::New(env, "Expected file path string").ThrowAsJavaScriptException();
    return env.Undefined();
  }

  std::string path = info[0].As<String>().Utf8Value();
  SDL_AudioSpec spec;
  Uint8* audio_buf;
  Uint32 audio_len;

  bool result = SDL_LoadWAV(path.c_str(), &spec, &audio_buf, &audio_len);
  if (!result) {
    return env.Null();
  }

  // Create ArrayBuffer with the audio data
  ArrayBuffer buffer = ArrayBuffer::New(env, audio_len);
  memcpy(buffer.Data(), audio_buf, audio_len);
  SDL_free(audio_buf);

  // Return object with spec and buffer
  Object returnObj = Object::New(env);
  Object specObj = Object::New(env);
  specObj.Set("format", Number::New(env, spec.format));
  specObj.Set("channels", Number::New(env, spec.channels));
  specObj.Set("freq", Number::New(env, spec.freq));

  returnObj.Set("spec", specObj);
  returnObj.Set("buffer", buffer);
  returnObj.Set("length", Number::New(env, audio_len));

  return returnObj;
}

Object Init (Env env, Object exports) {
  exports.Set("init", Function::New(env, Wrap_SDL_Init));
  exports.Set("quit", Function::New(env, Wrap_SDL_Quit));
  exports.Set("getError", Function::New(env, Wrap_SDL_GetError));
  exports.Set("delay", Function::New(env, Wrap_SDL_Delay));

  // Video functions
  exports.Set("createWindow", Function::New(env, Wrap_SDL_CreateWindow));
  exports.Set("destroyWindow", Function::New(env, Wrap_SDL_DestroyWindow));
  exports.Set("createRenderer", Function::New(env, Wrap_SDL_CreateRenderer));
  exports.Set("destroyRenderer", Function::New(env, Wrap_SDL_DestroyRenderer));
  exports.Set("setRenderDrawColor", Function::New(env, Wrap_SDL_SetRenderDrawColor));
  exports.Set("renderClear", Function::New(env, Wrap_SDL_RenderClear));
  exports.Set("renderRect", Function::New(env, Wrap_SDL_RenderRect));
  exports.Set("renderPresent", Function::New(env, Wrap_SDL_RenderPresent));
  exports.Set("pollEvent", Function::New(env, Wrap_SDL_PollEvent));

  // Audio device functions
  exports.Set("openAudioDevice", Function::New(env, Wrap_SDL_OpenAudioDevice));
  exports.Set("closeAudioDevice", Function::New(env, Wrap_SDL_CloseAudioDevice));
  exports.Set("getAudioDeviceName", Function::New(env, Wrap_SDL_GetAudioDeviceName));
  exports.Set("pauseAudioDevice", Function::New(env, Wrap_SDL_PauseAudioDevice));
  exports.Set("resumeAudioDevice", Function::New(env, Wrap_SDL_ResumeAudioDevice));
  exports.Set("audioDevicePaused", Function::New(env, Wrap_SDL_AudioDevicePaused));
  exports.Set("setAudioDeviceGain", Function::New(env, Wrap_SDL_SetAudioDeviceGain));
  exports.Set("getAudioDeviceGain", Function::New(env, Wrap_SDL_GetAudioDeviceGain));

  // Audio stream functions
  exports.Set("createAudioStream", Function::New(env, Wrap_SDL_CreateAudioStream));
  exports.Set("destroyAudioStream", Function::New(env, Wrap_SDL_DestroyAudioStream));
  exports.Set("bindAudioStream", Function::New(env, Wrap_SDL_BindAudioStream));
  exports.Set("unbindAudioStream", Function::New(env, Wrap_SDL_UnbindAudioStream));
  exports.Set("putAudioStreamData", Function::New(env, Wrap_SDL_PutAudioStreamData));
  exports.Set("getAudioStreamData", Function::New(env, Wrap_SDL_GetAudioStreamData));
  exports.Set("getAudioStreamAvailable", Function::New(env, Wrap_SDL_GetAudioStreamAvailable));
  exports.Set("flushAudioStream", Function::New(env, Wrap_SDL_FlushAudioStream));
  exports.Set("clearAudioStream", Function::New(env, Wrap_SDL_ClearAudioStream));
  exports.Set("loadWAV", Function::New(env, Wrap_SDL_LoadWAV));

  // Export SDL constants
  exports.Set("INIT_VIDEO", Number::New(env, SDL_INIT_VIDEO));
  exports.Set("INIT_AUDIO", Number::New(env, SDL_INIT_AUDIO));
  exports.Set("WINDOWPOS_CENTERED", Number::New(env, SDL_WINDOWPOS_CENTERED));
  exports.Set("WINDOW_RESIZABLE", Number::New(env, SDL_WINDOW_RESIZABLE));
  exports.Set("EVENT_QUIT", Number::New(env, SDL_EVENT_QUIT));

  // Audio constants
  exports.Set("AUDIO_DEVICE_DEFAULT_PLAYBACK", Number::New(env, SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK));
  exports.Set("AUDIO_DEVICE_DEFAULT_RECORDING", Number::New(env, SDL_AUDIO_DEVICE_DEFAULT_RECORDING));

  // Audio formats
  exports.Set("AUDIO_U8", Number::New(env, SDL_AUDIO_U8));
  exports.Set("AUDIO_S8", Number::New(env, SDL_AUDIO_S8));
  exports.Set("AUDIO_S16LE", Number::New(env, SDL_AUDIO_S16LE));
  exports.Set("AUDIO_S16BE", Number::New(env, SDL_AUDIO_S16BE));
  exports.Set("AUDIO_S16", Number::New(env, SDL_AUDIO_S16));
  exports.Set("AUDIO_S32LE", Number::New(env, SDL_AUDIO_S32LE));
  exports.Set("AUDIO_S32BE", Number::New(env, SDL_AUDIO_S32BE));
  exports.Set("AUDIO_S32", Number::New(env, SDL_AUDIO_S32));
  exports.Set("AUDIO_F32LE", Number::New(env, SDL_AUDIO_F32LE));
  exports.Set("AUDIO_F32BE", Number::New(env, SDL_AUDIO_F32BE));
  exports.Set("AUDIO_F32", Number::New(env, SDL_AUDIO_F32));

  return exports;
}

NODE_API_MODULE(sdl, Init)

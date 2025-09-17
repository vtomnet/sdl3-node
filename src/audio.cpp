#include <napi.h>
#include <SDL3/SDL_audio.h>

using namespace Napi;

Value Wrap_SDL_GetNumAudioDrivers(const CallbackInfo& info) {
  Env env = info.Env();
  return Number::New(env, SDL_GetNumAudioDrivers());
}

Value Wrap_SDL_GetAudioDriver(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected a number for index").ThrowAsJavaScriptException();
    return env.Undefined();
  }
  int index = info[0].As<Number>().Int32Value();
  const char *result = SDL_GetAudioDriver(index);
  if (result == nullptr) {
    Error::New(env, SDL_GetError()).ThrowAsJavaScriptException();
    return env.Undefined();
  }
  return String::New(env, result);
}

// Similar wrappers for other audio functions like SDL_GetCurrentAudioDriver, SDL_GetAudioPlaybackDevices, SDL_GetAudioRecordingDevices, SDL_GetAudioDeviceName, SDL_GetAudioDeviceFormat, SDL_GetAudioDeviceChannelMap, SDL_OpenAudioDevice, SDL_IsAudioDevicePhysical, SDL_IsAudioDevicePlayback, SDL_PauseAudioDevice, SDL_ResumeAudioDevice, SDL_AudioDevicePaused, SDL_GetAudioDeviceGain, SDL_SetAudioDeviceGain, SDL_CloseAudioDevice, SDL_BindAudioStreams, SDL_BindAudioStream, SDL_UnbindAudioStreams, SDL_UnbindAudioStream, SDL_GetAudioStreamDevice, SDL_CreateAudioStream, SDL_GetAudioStreamProperties, SDL_GetAudioStreamFormat, SDL_SetAudioStreamFormat, SDL_GetAudioStreamFrequencyRatio, SDL_SetAudioStreamFrequencyRatio, SDL_GetAudioStreamGain, SDL_SetAudioStreamGain, SDL_GetAudioStreamInputChannelMap, SDL_GetAudioStreamOutputChannelMap

void InitAudio(Env env, Object exports) {
  exports.Set("GetNumAudioDrivers", Function::New(env, Wrap_SDL_GetNumAudioDrivers));
  exports.Set("GetAudioDriver", Function::New(env, Wrap_SDL_GetAudioDriver));
  // Add all other audio wrappers here...
}

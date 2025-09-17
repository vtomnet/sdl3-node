#include <napi.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_blendmode.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_clipboard.h>
#include <SDL3/SDL_vulkan.h>
#include <SDL3/SDL_metal.h>
#include <SDL3/SDL_camera.h>

using namespace Napi;

Value Wrap_SDL_GetNumVideoDrivers(const CallbackInfo& info) {
  Env env = info.Env();
  return Number::New(env, SDL_GetNumVideoDrivers());
}

Value Wrap_SDL_GetVideoDriver(const CallbackInfo& info) {
  Env env = info.Env();
  if (info.Length() != 1 || !info[0].IsNumber()) {
    TypeError::New(env, "Expected a number for index").ThrowAsJavaScriptException();
    return env.Undefined();
  }
  int index = info[0].As<Number>().Int32Value();
  const char *result = SDL_GetVideoDriver(index);
  if (result == nullptr) {
    Error::New(env, SDL_GetError()).ThrowAsJavaScriptException();
    return env.Undefined();
  }
  return String::New(env, result);
}

// Similar wrappers for other video functions like SDL_GetCurrentVideoDriver, SDL_GetSystemTheme, SDL_GetDisplays, SDL_GetPrimaryDisplay, SDL_GetDisplayProperties, SDL_GetDisplayName, SDL_GetDisplayBounds, SDL_GetDisplayUsableBounds, SDL_GetNaturalDisplayOrientation, SDL_GetCurrentDisplayOrientation, SDL_GetDisplayContentScale, SDL_GetFullscreenDisplayModes, SDL_GetClosestFullscreenDisplayMode, SDL_GetDesktopDisplayMode, SDL_GetCurrentDisplayMode, SDL_GetDisplayForPoint, SDL_GetDisplayForRect, SDL_GetDisplayForWindow, SDL_GetWindowPixelDensity, SDL_GetWindowDisplayScale, SDL_SetWindowFullscreenMode, SDL_GetWindowFullScreenMode, SDL_GetWindowICCProfile, SDL_GetWindowPixelFormat, SDL_GetWindows, SDL_CreateWindow, SDL_CreatePopupWindow, SDL_CreateWindowWithProperties, SDL_GetWindowID, SDL_GetWindowFromID, SDL_GetWindowParent, SDL_GetWindowProperties, SDL_GetWindowFlags, SDL_SetWindowTitle, SDL_GetWindowTitle, SDL_SetWindowIcon, SDL_SetWindowPosition, SDL_GetWindowPosition, SDL_SetWindowSize, SDL_GetWindowSize, SDL_GetWindowSafeArea, SDL_SetWindowAspectRatio, SDL_GetWindowAspectRatio, SDL_GetWindowBordersSize, SDL_GetWindowSizeInPixels, SDL_SetWindowMinimumSize, SDL_GetWindowMinimumSize, SDL_SetWindowMaximumSize, and more for renderer, pixels, blendmode, rect, surface, clipboard, vulkan, metal, camera.

void InitVideo(Env env, Object exports) {
  exports.Set("GetNumVideoDrivers", Function::New(env, Wrap_SDL_GetNumVideoDrivers));
  exports.Set("GetVideoDriver", Function::New(env, Wrap_SDL_GetVideoDriver));
  // Add all other video wrappers here...
}

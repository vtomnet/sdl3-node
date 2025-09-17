#include <napi.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_joystick.h>
#include <SDL3/SDL_gamepad.h>
#include <SDL3/SDL_touch.h>
#include <SDL3/SDL_pen.h>
#include <SDL3/SDL_sensor.h>
#include <SDL3/SDL_hidapi.h>
#include <SDL3/SDL_haptic.h>

using namespace Napi;

Value Wrap_SDL_PumpEvents(const CallbackInfo& info) {
  Env env = info.Env();
  SDL_PumpEvents();
  return env.Undefined();
}

Value Wrap_SDL_PollEvent(const CallbackInfo& info) {
  Env env = info.Env();
  SDL_Event event;
  bool result = SDL_PollEvent(&event);
  if (result) {
    // Convert event to JS object
    Object jsEvent = Object::New(env);
    jsEvent.Set("type", Number::New(env, event.type));
    // Add more fields based on type
    return jsEvent;
  }
  return env.Null();
}

// Similar wrappers for other input functions like SDL_PeepEvents, SDL_HasEvent, SDL_HasEvents, SDL_FlushEvent, SDL_FlushEvents, SDL_WaitEvent, SDL_WaitEventTimeout, SDL_PushEvent, SDL_SetEventFilter, SDL_GetEventFilter, SDL_AddEventWatch, SDL_RemoveEventWatch, SDL_FilterEvents, SDL_SetEventEnabled, SDL_EventEnabled, SDL_RegisterEvents, SDL_GetWindowFromEvent, SDL_GetEventDescription, and for keyboard, mouse, touch, gamepad, joystick, haptic, sensor, hidapi.

void InitInput(Env env, Object exports) {
  exports.Set("PumpEvents", Function::New(env, Wrap_SDL_PumpEvents));
  exports.Set("PollEvent", Function::New(env, Wrap_SDL_PollEvent));
  // Add all other input wrappers here...
}

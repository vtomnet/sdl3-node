#include <napi.h>
#include <SDL3/SDL.h>

// Wrapper for SDL_Init(flags)
Napi::Number InitSDL(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "A number (flags) is expected").ThrowAsJavaScriptException();
    }

    uint32_t flags = info[0].As<Napi::Number>().Uint32Value();
    int result = SDL_Init(flags);

    return Napi::Number::New(env, result);
}

// Wrapper for SDL_Quit()
void QuitSDL(const Napi::CallbackInfo& info) {
    SDL_Quit();
}

// Wrapper for SDL_GetError()
Napi::String GetErrorSDL(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    const char* errorMessage = SDL_GetError();
    return Napi::String::New(env, errorMessage);
}


// Addon entry point
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("init", Napi::Function::New(env, InitSDL));
    exports.Set("quit", Napi::Function::New(env, QuitSDL));
    exports.Set("getError", Napi::Function::New(env, GetErrorSDL));
    return exports;
}

NODE_API_MODULE(sdl_addon, Init)

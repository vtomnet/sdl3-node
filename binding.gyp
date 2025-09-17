{
  "targets": [
    {
      "target_name": "sdl",
      "sources": [
        "src/sdl3.cpp"
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "<!@(brew --prefix sdl3)/include"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")"
      ],
      "conditions": [
        ['OS=="mac"', {
          "link_settings": {
            "libraries": [
              "-L<!@(brew --prefix sdl3)/lib", "-lSDL3"
            ]
          }
        }]
      ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "xcode_settings": {
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
      }
    }
  ]
}

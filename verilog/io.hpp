#include <SDL3/SDL.h>
#include <cstdio>
#include <cstdlib>
#include <memory>

struct GUI {
    GUI();
    ~GUI();
    void frame();
    bool running{};
    SDL_Keycode keysym;
    SDL_Surface *surf;
    std::unique_ptr<unsigned char[]> pix;
    SDL_Window *win;
    SDL_Renderer *ren;
};

#define VALIDATE(cond)                                      \
    if (!(cond)) {                                          \
        fprintf(stderr, "Error: SDL: %s ", SDL_GetError()); \
        fprintf(stderr, "(%s:%d)\n", __FILE__, __LINE__);   \
        exit(1);                                            \
    }

inline GUI::GUI() {
    pix = std::make_unique<unsigned char[]>(16 * 1024);
    VALIDATE(pix);
    VALIDATE(SDL_Init(SDL_INIT_VIDEO));
    win = SDL_CreateWindow("hack", 512, 256, 0);
    VALIDATE(win);
    ren = SDL_CreateRenderer(win, NULL);
    VALIDATE(ren);
    surf = SDL_CreateSurfaceFrom(512, 256, SDL_PIXELFORMAT_INDEX1LSB, pix.get(), 64);
    VALIDATE(surf);
    VALIDATE(SDL_CreateSurfacePalette(surf));
    running = true;
}

inline GUI::~GUI() {
    SDL_DestroySurface(surf);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

inline void GUI::frame() {
    SDL_Event e;
    if (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_EVENT_QUIT: running = false; return;
        case SDL_EVENT_KEY_DOWN: keysym = e.key.key; break;
        case SDL_EVENT_KEY_UP: keysym = 0; break;
        }
    }
    VALIDATE(SDL_RenderClear(ren));
    auto tex = SDL_CreateTextureFromSurface(ren, surf);
    VALIDATE(tex);
    VALIDATE(SDL_RenderTexture(ren, tex, NULL, NULL));
    SDL_DestroyTexture(tex);
    VALIDATE(SDL_RenderPresent(ren));
    SDL_Delay(50);
}

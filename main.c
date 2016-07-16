#include <stdlib.h>

#include "types.c"
#include "explorer.c"

void init() {
  // initialize SDL video
  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    printf( "Unable to init SDL: %s\n", SDL_GetError() );
    exit(1);
  }
  atexit(SDL_Quit);
  screen = SDL_SetVideoMode(224, 136, 8, SDL_SWSURFACE|SDL_DOUBLEBUF);
  if (!screen) {
    exit(1);
  }
  loadRessources();
  discoverMap();
}

void pollEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        gameMode = quit;
        break;
      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
          gameMode = quit;
        break;
    }
    switch (gameMode) {
      case explorate : exploratePollEvent(event); break;
      case battle :    battlePoolEvent(event);    break;
    }
  }
}

int main() {
  init();

  while (gameMode != quit) {
    pollEvents();
    display3dView();
    if (gameMode == battle) {
      SDL_BlitSurface(monsterTexts[monster.id], NULL, screen, &monsterRects[monster.id]);
    }
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (keystate[SDLK_TAB]) {
      displayMinimap();
    }
    SDL_Flip(screen);
  }
  return 0;
}

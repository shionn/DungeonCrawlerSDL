#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <assert.h>

SDL_Surface* loadImage(char* file) {
  SDL_Surface* tmp = IMG_Load(file);
  SDL_Surface* optimized = SDL_DisplayFormat(tmp);
  SDL_FreeSurface(tmp);
  SDL_SetColorKey( optimized, SDL_SRCCOLORKEY | SDL_RLEACCEL , SDL_MapRGB(optimized->format, 255, 0, 255));
  return optimized;
}

/** 0
 * 3 1
 *  2  */

typedef struct {
  int x;
  int y;
  int d;
} PlayerType;


#define DOOR_H 2
#define DOOR_V 3
#define DOOR_CLOSED_H 4
#define DOOR_CLOSED_V 5
#define DOOR_TRAPPED_H 6
#define DOOR_TRAPPED_V 7

SDL_Surface* screen;
SDL_Surface* groundTexts[1];
SDL_Rect     groundRects[1];
SDL_Surface* wallTexts[13];
SDL_Rect     wallRects[13];
SDL_Rect     wallSrcRects[13];
SDL_Surface* doorTexts[13];
SDL_Rect     doorRects[13];
SDL_Rect     doorSrcRects[13];
PlayerType   player = {1,1,1};

int dungeon[19][19] =
{{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,2,0,0,0,0,0,1,0,0,0,0,0,0,0,1},
{1,0,1,1,1,0,0,0,0,0,1,1,1,2,1,0,1,0,1},
{1,0,1,0,2,0,0,0,0,0,1,0,0,0,1,0,1,0,1},
{1,0,1,1,1,2,1,2,1,1,1,0,0,0,1,1,1,0,1},
{1,0,2,0,0,0,1,0,0,0,2,0,0,0,1,0,0,0,1},
{1,0,1,0,0,0,1,0,0,0,1,2,1,1,1,0,1,0,1},
{1,0,2,0,0,0,1,0,0,0,1,1,1,1,1,0,1,0,1},
{1,0,1,0,0,0,1,0,0,0,1,1,1,1,1,0,1,0,1},
{1,0,1,0,0,0,2,0,0,0,2,0,0,0,1,0,1,0,1},
{1,0,1,1,1,1,1,1,1,1,1,2,1,0,1,0,1,0,1},
{1,0,1,1,1,1,1,0,0,0,0,0,1,0,1,0,1,0,1},
{1,0,1,1,1,1,1,0,0,0,0,0,1,0,1,0,1,1,1},
{1,0,0,0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,1},
{1,1,1,1,1,0,1,1,1,1,1,2,1,0,1,1,1,0,1},
{1,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,1},
{1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};


void loadWalls() {
  wallTexts[0] = loadImage("wall-0l.png");
  wallTexts[1] = loadImage("wall-0r.png");

  wallTexts[2] = loadImage("wall-1.png");
  wallTexts[3] =  wallTexts[2];
  wallTexts[4] =  wallTexts[2];

  wallTexts[5] = loadImage("wall-2.png");
  wallTexts[6] =  wallTexts[5];
  wallTexts[7] =  wallTexts[5];

  wallTexts[8] = loadImage("wall-3l.png");
  wallTexts[9] = loadImage("wall-3.png");
  wallTexts[10] = wallTexts[9];
  wallTexts[11] = wallTexts[9];
  wallTexts[12] = loadImage("wall-3r.png");

  wallRects[0].x = 0;                       wallRects[0].y = 0;
  wallRects[1].x = 32 + 160;                wallRects[1].y = 0;

  wallRects[2].x = 0;                       wallRects[2].y = 9;
  wallRects[3].x = 16;                      wallRects[3].y = 9;
  wallRects[4].x = 164;                     wallRects[4].y = 9;

  wallRects[5].x = 0;                       wallRects[5].y = 20;
  wallRects[6].x = 44+15;                   wallRects[6].y = 20;
  wallRects[7].x = 149;                     wallRects[7].y = 20;

  wallRects[8].x = 0;                       wallRects[8].y = 26;
  wallRects[9].x = 0;                       wallRects[9].y = 25;
  wallRects[10].x = 64;                     wallRects[10].y = 25;
  wallRects[11].x = 139;                    wallRects[11].y = 25;
  wallRects[12].x = 208;                    wallRects[12].y = 26;

  wallSrcRects[2].x = 192;                  wallSrcRects[2].y = 0;
  wallSrcRects[2].w = 60;                   wallSrcRects[2].h = 111;
  wallSrcRects[3].x = 32;                   wallSrcRects[3].y = 0;
  wallSrcRects[3].w = 192;                  wallSrcRects[3].h = 111;
  wallSrcRects[4].x = 4;                    wallSrcRects[4].y = 0;
  wallSrcRects[4].w = 60;                   wallSrcRects[4].h = 111;

  wallSrcRects[5].x = 61;                   wallSrcRects[5].y = 0;
  wallSrcRects[5].w = 75;                   wallSrcRects[5].h = 71;
  wallSrcRects[6].x = 15;                   wallSrcRects[6].y = 0;
  wallSrcRects[6].w = 106;                  wallSrcRects[6].h = 71;
  wallSrcRects[7].x = 0;                    wallSrcRects[7].y = 0;
  wallSrcRects[7].w = 75;                   wallSrcRects[7].h = 71;

  wallSrcRects[9].x = 32;                   wallSrcRects[9].y = 0;
  wallSrcRects[9].w = 85;                   wallSrcRects[9].h = 51;
  wallSrcRects[10].x = 11;                  wallSrcRects[10].y = 0;
  wallSrcRects[10].w = 95;                  wallSrcRects[10].h = 51;
  wallSrcRects[11].x = 0;                   wallSrcRects[11].y = 0;
  wallSrcRects[11].w = 85;                  wallSrcRects[11].h = 51;
}

int map(int x,int y) {
  if (x<0 || x>18 || y<0 || y>18) return 1;
  return dungeon[y][x];
}


void init() {
  // initialize SDL video
  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    printf( "Unable to init SDL: %s\n", SDL_GetError() );
    exit(1);
  }
  atexit(SDL_Quit);
  screen = SDL_SetVideoMode(224, 136, 8, SDL_SWSURFACE|SDL_DOUBLEBUF);
//  SDL_RenderSetLogicalSize(screen, 256, 136);
//  SDL_SetHint(SDL_RENHINT_RENDER_SCALE_QUALITY, 0);
  if (!screen) {
    exit(1);
  }

  groundTexts[0] = loadImage("ground.png");
  groundRects[0].x = 0; groundRects[0].y = 136-70;

  loadWalls();

  doorTexts[0] = loadImage("door-1.png");
  doorTexts[1] = loadImage("door-1l.png");
  doorTexts[2] = doorTexts[0];
  doorTexts[3] = loadImage("door-1r.png");
  doorTexts[4] = doorTexts[0];


}

int main() {

  init();

  int done = 0;
  while (!done) {


    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          done = 1;
          break;
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE)
            done = 1;
          if (event.key.keysym.sym == SDLK_UP) {
            player.x += player.d == 1 ?  1 : player.d == 3 ? -1 : 0;
            player.y += player.d == 0 ? -1 : player.d == 2 ?  1 : 0;
          }
          if (event.key.keysym.sym == SDLK_RIGHT) player.d = (player.d +1) % 4;
          if (event.key.keysym.sym == SDLK_LEFT) player.d = (player.d +3) % 4;
          break;
      }
    }

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
    SDL_BlitSurface(groundTexts[0], NULL, screen, &groundRects[0]);

    int fx = player.d == 1 ?  1 : player.d == 3 ? -1 : 0;
    int fy = player.d == 0 ? -1 : player.d == 2 ?  1 : 0;
    int sx = player.d == 0 ?  1 : player.d == 2 ? -1 : 0; // -fy
    int sy = player.d == 1 ?  1 : player.d == 3 ? -1 : 0; // fx

    // Affichage mur far en profondeur 3
    if (map(player.x+fx*3-sx*2, player.y+fy*3-sy*2))
      SDL_BlitSurface(wallTexts[8],  NULL, screen, &wallRects[8]);
    if (map(player.x+fx*3+sx*2, player.y+fy*3+sy*2))
      SDL_BlitSurface(wallTexts[12], NULL, screen, &wallRects[12]);
    // affichage en 3
    if (map(player.x+fx*3-sx, player.y+fy*3-sy))
      SDL_BlitSurface(wallTexts[9],  &wallSrcRects[9],  screen, &wallRects[9]);
    if (map(player.x+fx*3+sx, player.y+fy*3+sy))
      SDL_BlitSurface(wallTexts[11], &wallSrcRects[11], screen, &wallRects[11]);
    if (map(player.x+fx*3, player.y+fy*3))
      SDL_BlitSurface(wallTexts[10], &wallSrcRects[10], screen, &wallRects[10]);

    // affichage en 2
    if (map(player.x+fx*2-sx, player.y+fy*2-sy))
      SDL_BlitSurface(wallTexts[5], &wallSrcRects[5], screen, &wallRects[5]);
    if (map(player.x+fx*2+sx, player.y+fy*2+sy))
      SDL_BlitSurface(wallTexts[7], &wallSrcRects[7], screen, &wallRects[7]);
    if (map(player.x+fx*2, player.y+fy*2))
      SDL_BlitSurface(wallTexts[6], &wallSrcRects[6], screen, &wallRects[6]);

    // affichage en 1
    if (map(player.x+fx-sx, player.y+fy-sy))
      SDL_BlitSurface(wallTexts[2], &wallSrcRects[2], screen, &wallRects[2]);
    if (map(player.x+fx+sx, player.y+fy+sy))
      SDL_BlitSurface(wallTexts[4], &wallSrcRects[4], screen, &wallRects[4]);
    if (map(player.x+fx, player.y+fy))
      SDL_BlitSurface(wallTexts[3], &wallSrcRects[3], screen, &wallRects[3]);

    // affichage en 0
    if (map(player.x-sx, player.y-sy))
      SDL_BlitSurface(wallTexts[0], NULL, screen, &wallRects[0]);
    if (map(player.x+sx, player.y+sy))
      SDL_BlitSurface(wallTexts[1], NULL, screen, &wallRects[1]);


    SDL_Flip(screen);
  }
  return 0;
}

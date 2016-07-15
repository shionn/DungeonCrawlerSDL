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

#define DUNGEON_W 23

#define NOTHING     0x00000000

#define BLOCKED     0x00000001
#define ROOM        0x00000002
#define CORRIDOR    0x00000004
#define VISITED     0x00000008
#define PERIMETER   0x00000010
#define ENTRANCE    0x00000020
// peu etre supprimé pour opti
#define ROOM_ID     0x0000FFC0

#define ARCH        0x00010000
#define DOOR        0x00020000
#define LOCKED      0x00040000
#define TRAPPED     0x00080000
#define SECRET      0x00100000
#define PORTC       0x00200000
#define STAIR_DN    0x00400000
#define STAIR_UP    0x00800000

// peu etre supprimé pour opti
#define LABEL       0xFF000000

#define OPENSPACE   ( ROOM | CORRIDOR )
#define DOORSPACE   ( ARCH | DOOR | LOCKED | TRAPPED | SECRET | PORTC )
#define ESPACE      ( ENTRANCE | DOORSPACE | 0xFF000000 )
#define STAIRS      ( STAIR_DN | STAIR_UP )

//my $BLOCK_ROOM  = $BLOCKED | $ROOM;
//my $BLOCK_CORR  = $BLOCKED | $PERIMETER | $CORRIDOR;
//my $BLOCK_DOOR  = $BLOCKED | $DOORSPACE;

SDL_Surface* screen;
SDL_Surface* groundTexts[1];
SDL_Rect     groundRects[1];
SDL_Surface* wallTexts[13];
SDL_Rect     wallRects[13];
SDL_Rect     wallSrcRects[13];
SDL_Surface* doorTexts[17];
SDL_Rect     doorRects[17];
SDL_Rect     doorSrcRects[17];
PlayerType   player = {1,1,1};

long dungeon[DUNGEON_W][DUNGEON_W] =
{{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x10,0x10,0x10,0x10,0x10,0x0,0x0,0x0,0x0,0x0,0x0},
{0x0,0x4,0x4,0x4,0x4,0x4,0x0,0x4,0x4,0x4,0x0,0x0,0x10,0x86,0x86,0x86,0x23200004,0x4,0x4,0x4,0x4,0x4,0x0},
{0x0,0x4,0x0,0x0,0x10,0x10004,0x10,0x10,0x10,0x78040004,0x10,0x10,0x10,0x86,0x82,0x82,0x10,0x4,0x0,0x0,0x0,0x4,0x0},
{0x0,0x4,0x4,0x4,0x74080020,0x206,0x202,0x206,0x23200004,0xc6,0xc6,0xc6,0x10,0x86,0x32000086,0x86,0x10,0x4,0x4,0x4,0x0,0x4,0x0},
{0x0,0x0,0x0,0x0,0x10,0x206,0x38000202,0x206,0x10,0xc2,0xc2,0xc6,0x10,0x82,0x82,0x82,0x10,0x6f020020,0x10,0x78040004,0x10,0x4,0x0},
{0x0,0x4,0x4,0x4,0x10,0x206,0x206,0x206,0x10,0xc6,0x330000c2,0xc6,0x73100004,0x86,0x86,0x86,0x74080004,0x186,0x182,0x186,0x10,0x4,0x0},
{0x0,0x4,0x0,0x0,0x10,0x10,0x10,0x74080020,0x10,0xc6,0xc2,0xc6,0x10,0x10,0x10,0x10,0x10,0x186,0x36000182,0x182,0x10,0x4,0x0},
{0x0,0x4,0x0,0x4,0x4,0x4,0x4,0x4,0x10,0xc6,0xc6,0xc6,0x10,0x4,0x4,0x4,0x10,0x186,0x186,0x186,0x10004,0x4,0x0},
{0x0,0x4,0x0,0x4,0x0,0x0,0x0,0x4,0x10,0x10,0x10,0x10,0x10,0x4,0x0,0x0,0x10,0x10,0x10,0x10,0x10,0x4,0x0},
{0x0,0x4,0x4,0x4,0x4,0x4,0x0,0x4,0x0,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x75800004,0x0,0x4,0x0},
{0x10,0x10,0x10,0x10,0x10,0x4,0x0,0x4,0x0,0x4,0x0,0x0,0x10,0x23200020,0x10,0x10,0x10,0x4,0x0,0x0,0x0,0x4,0x0},
{0x10,0x146,0x146,0x146,0x10,0x4,0x0,0x4,0x4,0x4,0x0,0x0,0x10,0x46,0x46,0x46,0x10,0x4,0x4,0x4,0x4,0x4,0x0},
{0x10,0x146,0x142,0x142,0x10,0x4,0x0,0x0,0x10,0x10,0x10,0x10,0x10,0x46,0x42,0x42,0x10,0x0,0x0,0x0,0x0,0x0,0x0},
{0x10,0x146,0x35000146,0x146,0x78040004,0x4,0x0,0x0,0x0,0x246,0x246,0x246,0x10,0x46,0x31000046,0x46,0x6f020004,0x4,0x4,0x4,0x4,0x4,0x0},
{0x10,0x146,0x142,0x142,0x10,0x0,0x0,0x0,0x10,0x242,0x39000242,0x246,0x10,0x42,0x42,0x42,0x10,0x4,0x0,0x0,0x0,0x4,0x0},
{0x10,0x146,0x142,0x146,0x10004,0x4,0x4,0x4,0x23200004,0x246,0x246,0x246,0x23200004,0x46,0x46,0x46,0x10,0x4,0x0,0x4,0x0,0x4,0x0},
{0x10,0x6f020004,0x10,0x10,0x10,0x4,0x10,0x10,0x10,0x10020,0x10,0x10,0x10,0x10,0x10,0x74080004,0x10,0x4,0x0,0x4,0x0,0x4,0x0},
{0x0,0x4,0x4,0x4,0x4,0x4,0x10,0x106,0x106,0x106,0x78040004,0x4,0x10,0x1c6,0x1c2,0x1c6,0x10,0x4,0x4,0x4,0x0,0x4,0x0},
{0x0,0x0,0x0,0x0,0x0,0x0,0x10,0x106,0x34000102,0x106,0x10,0x4,0x10,0x1c6,0x370001c2,0x1c6,0x10,0x0,0x0,0x4,0x0,0x4,0x0},
{0x0,0x0,0x0,0x0,0x0,0x4,0x6f020004,0x106,0x102,0x106,0x10,0x4,0x10,0x1c6,0x1c2,0x1c6,0x6f020020,0x64400004,0x4,0x4,0x0,0x4,0x0},
{0x0,0x0,0x0,0x0,0x0,0x4,0x10,0x10,0x10,0x10,0x10,0x0,0x10,0x78040004,0x10,0x10004,0x10,0x0,0x0,0x0,0x0,0x4,0x0},
{0x0,0x0,0x0,0x0,0x0,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x0},
{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};

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

long map(int x,int y) {
  if (x<0 || x>=DUNGEON_W || y<0 || y>=DUNGEON_W) return 0;
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

  doorTexts[0]  = loadImage("door-1.png");
  doorTexts[1]  = loadImage("door-1l.png");
  doorTexts[2]  = doorTexts[0];
  doorTexts[3]  = loadImage("door-1r.png");
  doorTexts[4]  = doorTexts[0];

  doorTexts[5]  = loadImage("door-2.png");
  doorTexts[6]  = loadImage("door-2l.png");
  doorTexts[7]  = doorTexts[5];
  doorTexts[8]  = loadImage("door-2r.png");
  doorTexts[9]  = doorTexts[5];

  doorTexts[10] = loadImage("door-3ll.png");
  doorTexts[11] = loadImage("door-3.png");
  doorTexts[12] = loadImage("door-3l.png");
  doorTexts[13] = doorTexts[11];
  doorTexts[14] = loadImage("door-3r.png");
  doorTexts[15] = doorTexts[11];
  doorTexts[16] = loadImage("door-3rr.png");

  doorRects[0].x = 0;       doorRects[0].y = 17;
  doorRects[1].x = 43;      doorRects[1].y = 15;
  doorRects[2].x = 64;      doorRects[2].y = 17;
  doorRects[3].x = 149;     doorRects[3].y = 15;
  doorRects[4].x = 193;     doorRects[4].y = 17;

  doorRects[5].x = 0;       doorRects[5].y = 24;
  doorRects[6].x = 48;      doorRects[6].y = 22;
  doorRects[7].x = 80;      doorRects[7].y = 24;
  doorRects[8].x = 128;     doorRects[8].y = 22;
  doorRects[9].x = 165;     doorRects[9].y = 24;

  doorRects[10].x = 0;      doorRects[10].y = 29;
  doorRects[11].x = 27;     doorRects[11].y = 29;
  doorRects[12].x = 64;     doorRects[12].y = 27;
  doorRects[13].x = 88;     doorRects[13].y = 28;
  doorRects[14].x = 128;    doorRects[14].y = 27;
  doorRects[15].x = 160;    doorRects[15].y = 29;
  doorRects[16].x = 192;    doorRects[16].y = 29;


  doorSrcRects[0].x = 65;   doorSrcRects[0].y = 0;
  doorSrcRects[0].w = 31;   doorSrcRects[0].h = 88;
  doorSrcRects[4].x = 0;    doorSrcRects[4].y = 0;
  doorSrcRects[4].w = 31;   doorSrcRects[4].h = 88;

  doorSrcRects[5].x = 1;    doorSrcRects[5].y = 0;
  doorSrcRects[5].w = 59;   doorSrcRects[5].h = 61;
  doorSrcRects[9].x = 0;    doorSrcRects[9].y = 0;
  doorSrcRects[9].w = 59;   doorSrcRects[9].h = 61;

  doorSrcRects[11].x = 0;   doorSrcRects[11].y = 0;
  doorSrcRects[11].w = 39;  doorSrcRects[11].h = 41;
  doorSrcRects[15].x = 9;   doorSrcRects[15].y = 0;
  doorSrcRects[15].w = 39;  doorSrcRects[15].h = 41;


}

void display3dView() {
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
    SDL_BlitSurface(groundTexts[0], NULL, screen, &groundRects[0]);

    int fx = player.d == 1 ?  1 : player.d == 3 ? -1 : 0;
    int fy = player.d == 0 ? -1 : player.d == 2 ?  1 : 0;
    int sx = player.d == 0 ?  1 : player.d == 2 ? -1 : 0; // -fy
    int sy = player.d == 1 ?  1 : player.d == 3 ? -1 : 0; // fx

    // Affichage mur far en profondeur 3
    int data = 0;
    if ( !(map(player.x+fx*3-sx*2, player.y+fy*3-sy*2) & OPENSPACE) )
      SDL_BlitSurface(wallTexts[8],  NULL, screen, &wallRects[8]);
    if ( !(map(player.x+fx*3+sx*2, player.y+fy*3+sy*2) & OPENSPACE) )
      SDL_BlitSurface(wallTexts[12], NULL, screen, &wallRects[12]);
    // affichage en 3
    data = map(player.x+fx*3-sx, player.y+fy*3-sy);
    if ( !(data & OPENSPACE) )
      SDL_BlitSurface(wallTexts[9],  &wallSrcRects[9],  screen, &wallRects[9]);
    else if (data & (ARCH | DOOR | LOCKED)) {
      SDL_BlitSurface(doorTexts[10], NULL, screen, &doorRects[10]);
      if ( data & (DOOR | LOCKED) ) {
        SDL_BlitSurface(doorTexts[11], &doorSrcRects[11], screen, &doorRects[11]);
      }
    }
    data = map(player.x+fx*3+sx, player.y+fy*3+sy);
    if ( !(data & OPENSPACE) )
      SDL_BlitSurface(wallTexts[11], &wallSrcRects[11], screen, &wallRects[11]);
    else if (data & (ARCH | DOOR | LOCKED)) {
      SDL_BlitSurface(doorTexts[16], NULL, screen, &doorRects[16]);
      if ( data & (DOOR | LOCKED) ) {
        SDL_BlitSurface(doorTexts[15], &doorSrcRects[15], screen, &doorRects[15]);
      }
    }
    data = map(player.x+fx*3, player.y+fy*3);
    if ( !(data & OPENSPACE) )
      SDL_BlitSurface(wallTexts[10], &wallSrcRects[10], screen, &wallRects[10]);
    else if (data & (ARCH | DOOR | LOCKED)) {
      SDL_BlitSurface(doorTexts[12], NULL, screen, &doorRects[12]);
      SDL_BlitSurface(doorTexts[14], NULL, screen, &doorRects[14]);
      if ( data & (DOOR | LOCKED) ) {
        SDL_BlitSurface(doorTexts[13], NULL, screen, &doorRects[13]);
      }
    }

    // affichage en 2
    data = map(player.x+fx*2-sx, player.y+fy*2-sy);
    if ( !(data & OPENSPACE) )
      SDL_BlitSurface(wallTexts[5], &wallSrcRects[5], screen, &wallRects[5]);
    else if ( data & (DOOR | LOCKED) )
      SDL_BlitSurface(doorTexts[5], &doorSrcRects[5], screen, &doorRects[5]);
    data = map(player.x+fx*2+sx, player.y+fy*2+sy);
    if ( !(data & OPENSPACE) )
      SDL_BlitSurface(wallTexts[7], &wallSrcRects[7], screen, &wallRects[7]);
    else if ( data & (DOOR | LOCKED) )
      SDL_BlitSurface(doorTexts[9], &doorSrcRects[9], screen, &doorRects[9]);
    data = map(player.x+fx*2, player.y+fy*2);
    if ( !(data & OPENSPACE) )
      SDL_BlitSurface(wallTexts[6], &wallSrcRects[6], screen, &wallRects[6]);
    else if ( data & (ARCH | DOOR | LOCKED) ) {
      SDL_BlitSurface(doorTexts[6], NULL, screen, &doorRects[6]);
      SDL_BlitSurface(doorTexts[8], NULL, screen, &doorRects[8]);
      if ( data & (DOOR | LOCKED) ) {
        SDL_BlitSurface(doorTexts[7], NULL, screen, &doorRects[7]);
      }
    }

    // affichage en 1
    data = map(player.x+fx-sx, player.y+fy-sy);
    if ( !(data & OPENSPACE) )
      SDL_BlitSurface(wallTexts[2], &wallSrcRects[2], screen, &wallRects[2]);
    else if ( data & (DOOR | LOCKED) )
      SDL_BlitSurface(doorTexts[0], &doorSrcRects[0], screen, &doorRects[0]);
    data = map(player.x+fx+sx, player.y+fy+sy);
    if ( !(data & OPENSPACE) )
      SDL_BlitSurface(wallTexts[4], &wallSrcRects[4], screen, &wallRects[4]);
    else if ( data & (DOOR | LOCKED) )
      SDL_BlitSurface(doorTexts[4], &doorSrcRects[4], screen, &doorRects[4]);
    data = map(player.x+fx, player.y+fy);
    if ( !(data & OPENSPACE) )
      SDL_BlitSurface(wallTexts[3], &wallSrcRects[3], screen, &wallRects[3]);
    else if ( data & (ARCH | DOOR | LOCKED) ) {
      SDL_BlitSurface(doorTexts[1], NULL, screen, &doorRects[1]);
      SDL_BlitSurface(doorTexts[3], NULL, screen, &doorRects[3]);
      if ( data & (DOOR | LOCKED) ) {
        SDL_BlitSurface(doorTexts[2], NULL, screen, &doorRects[2]);
      }
    }

    // affichage en 0
    if ( !(map(player.x-sx, player.y-sy) & OPENSPACE) )
      SDL_BlitSurface(wallTexts[0], NULL, screen, &wallRects[0]);
    if ( !(map(player.x+sx, player.y+sy) & OPENSPACE) )
      SDL_BlitSurface(wallTexts[1], NULL, screen, &wallRects[1]);
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
            int nx = player.x + (player.d == 1 ?  1 : player.d == 3 ? -1 : 0);
            int ny = player.y + (player.d == 0 ? -1 : player.d == 2 ?  1 : 0);
            if (map(nx,ny) & OPENSPACE) {
              player.x = nx;
              player.y = ny;
              for (int y=player.y-1;y<=player.y+1;y++)
                for (int x=player.x-1;x<=player.x+1;x++) {
                  dungeon[y][x] = dungeon[y][x] | VISITED;
                }
            }
          }
          if (event.key.keysym.sym == SDLK_DOWN) {
            int nx = player.x - (player.d == 1 ?  1 : player.d == 3 ? -1 : 0);
            int ny = player.y - (player.d == 0 ? -1 : player.d == 2 ?  1 : 0);
            if (map(nx,ny) & OPENSPACE) {
              player.x = nx;
              player.y = ny;
              for (int y=player.y-1;y<=player.y+1;y++)
                for (int x=player.x-1;x<=player.x+1;x++) {
                  dungeon[y][x] = dungeon[y][x] | VISITED;
                }
            }
          }
          if (event.key.keysym.sym == SDLK_RIGHT) player.d = (player.d +1) % 4;
          if (event.key.keysym.sym == SDLK_LEFT) player.d = (player.d +3) % 4;
          break;
      }
    }

    Uint8 *keystate = SDL_GetKeyState(NULL);

    display3dView();
    if (keystate[SDLK_TAB]) {
      SDL_Rect box = {0, 0, 6, 6};
      for (int y=player.y-9;y<=player.y+9;y++)
        for (int x=player.x-9;x<=player.x+9;x++) {
          if ( map(x,y) & VISITED ) {
            box.x = 224 / 2 + (x-player.x)*6 - 3;
            box.y = 136 / 2 + (y-player.y)*6 - 3;
            box.w = box.h = 7;
            SDL_FillRect(screen,&box,SDL_MapRGB(screen->format,255,255,255));
            box.x++; box.y++; box.w = box.h = 5;
            if (! ( map(x,y) & OPENSPACE ) ) {
              SDL_FillRect(screen,&box,SDL_MapRGB(screen->format,96,96,96));
            } else  {
              SDL_FillRect(screen,&box,SDL_MapRGB(screen->format,192,192,192));
            }
          }
        }
    }

    SDL_Flip(screen);
  }
  return 0;
}

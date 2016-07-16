
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#ifndef __DUNGEON_TYPE__
#define __DUNGEON_TYPE__

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

typedef struct {
  int x;
  int y;
  int d;

  int pv;
  int f;
  int xp;
} PlayerType;

typedef struct {
  int pv;
  int damage[2];
  int xp;
} MonsterType;

typedef struct {
  int id;
  int pv;
} MonsterEngageType;

typedef enum {
  explorate,
  battle,
  quit
} GameModeType;



SDL_Surface* screen;
SDL_Surface* groundTexts[2];
SDL_Rect     groundRects[2];
SDL_Surface* monsterTexts[8];
SDL_Rect     monsterRects[8];
SDL_Surface* wallTexts[13];
SDL_Rect     wallRects[13];
SDL_Rect     wallSrcRects[13];
SDL_Surface* doorTexts[17];
SDL_Rect     doorRects[17];
SDL_Rect     doorSrcRects[17];

PlayerType        player = {17,19,1, 20,3,0};
GameModeType      gameMode = explorate;
MonsterEngageType monster;

MonsterType  monsters[9] = {
  {5, {2,3}, 3}, //IMP
};


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

long map(int x,int y) {
  if (x<0 || x>=DUNGEON_W || y<0 || y>=DUNGEON_W) return 0;
  return dungeon[y][x];
}


SDL_Surface* loadImage(char* file) {
  SDL_Surface* tmp = IMG_Load(file);
  SDL_Surface* optimized = SDL_DisplayFormat(tmp);
  SDL_FreeSurface(tmp);
  SDL_SetColorKey( optimized, SDL_SRCCOLORKEY | SDL_RLEACCEL , SDL_MapRGB(optimized->format, 255, 0, 255));
  return optimized;
}

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

void loadDoors() {
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

void loadEnnemies() {
  monsterTexts[0] = loadImage("imp.png");
  monsterRects[0].x = (224-monsterTexts[0]->w)/2;
  monsterRects[0].y = 131-monsterTexts[0]->h;
  monsterTexts[1] = loadImage("zombie.png");
  monsterRects[1].x = (224-monsterTexts[1]->w)/2;
  monsterRects[1].y = 131-monsterTexts[1]->h;
  monsterTexts[2] = loadImage("shadow_soul.png");
  monsterRects[2].x = (224-monsterTexts[2]->w)/2;
  monsterRects[2].y = 131-monsterTexts[2]->h;
  monsterTexts[3] = loadImage("skeleton.png");
  monsterRects[3].x = (224-monsterTexts[3]->w)/2;
  monsterRects[3].y = 131-monsterTexts[3]->h;
  monsterTexts[4] = loadImage("mimic.png");
  monsterRects[4].x = (224-monsterTexts[4]->w)/2;
  monsterRects[4].y = 131-monsterTexts[4]->h;
  monsterTexts[5] = loadImage("druid.png");
  monsterRects[5].x = (224-monsterTexts[5]->w)/2;
  monsterRects[5].y = 131-monsterTexts[5]->h;
  monsterTexts[6] = loadImage("shadow_tendrils.png");
  monsterRects[6].x = (224-monsterTexts[6]->w)/2;
  monsterRects[6].y = 131-monsterTexts[6]->h;
  monsterTexts[7] = loadImage("death_speaker.png");
  monsterRects[7].x = (224-monsterTexts[7]->w)/2;
  monsterRects[7].y = 131-monsterTexts[7]->h;
}

void loadRessources() {
  groundTexts[0] = loadImage("ground.png");
  groundRects[0].x = 0; groundRects[0].y = 136-70;
  groundTexts[1] = loadImage("roof.png");
  groundRects[1].x = 0; groundRects[1].y = 0;

  loadWalls();
  loadDoors();
  loadEnnemies();
}

#endif

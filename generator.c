#include <stdlib.h>

#define DUNGEON_W 32
#define DUNGEON_MIN_ROOM 2

typedef struct {
  int sx;
  int ex;
  int sy;
  int ey;
} ZoneType;

void _d_fill(int dungeon[DUNGEON_W][DUNGEON_W],int type, ZoneType zone) {
  for (int x=zone.sx;x<=zone.ex;x++) {
    for (int y=zone.sy;y<=zone.ey;y++) {
      dungeon[x][y]=type;
    }
  }
}

int _d_shouldSplit(ZoneType zone) {
  return zone.ex-zone.sx >= DUNGEON_MIN_ROOM * 2 + 3 || zone.ey-zone.sy >= DUNGEON_MIN_ROOM * 2 +3;
}

void _d_splitVertical(int dungeon[DUNGEON_W][DUNGEON_W], ZoneType zone) {
  int col = rand()%(zone.ex-zone.sx-DUNGEON_MIN_ROOM*2)+DUNGEON_MIN_ROOM+zone.sx+1;
  ZoneType left =  {zone.sx, col, zone.sy, zone.ey};
  ZoneType right = {col, zone.ex, zone.sy, zone.ey};
  _d_generate(dungeon, left);
  _d_generate(dungeon, right);
}

void _d_splitHorizontal(int dungeon[DUNGEON_W][DUNGEON_W], ZoneType zone) {
  int row = rand()%(zone.ey-zone.sy-DUNGEON_MIN_ROOM*2)+DUNGEON_MIN_ROOM+zone.sy+1;
  ZoneType top = {zone.sx, zone.ex, zone.sy, row};
  ZoneType bot = {zone.sx, zone.ex, row, zone.ey};
  _d_generate(dungeon, top);
  _d_generate(dungeon, bot);
}

void _d_generate(int dungeon[DUNGEON_W][DUNGEON_W], ZoneType zone) {
  if (_d_shouldSplit(zone)) {
    if (zone.ex-zone.sx > zone.ey-zone.sy) {
      _d_splitVertical(dungeon, zone);
    } else {
      _d_splitHorizontal(dungeon, zone);
    }
  } else {
    ZoneType room = {zone.sx+1, zone.ex-1, zone.sy+1, zone.ey-1};
    _d_fill(dungeon, 0, room);
  }
}


void _d_startGenerate(int dungeon[DUNGEON_W][DUNGEON_W]) {
  ZoneType all = {0,DUNGEON_W-1,0,DUNGEON_W-1};
  _d_fill(dungeon,1,all);
  _d_generate(dungeon,all);
}

void _d_debug(int dungeon[DUNGEON_W][DUNGEON_W]) {
  for (int j=0;j<DUNGEON_W;j++) {
    for (int i=0;i<DUNGEON_W;i++) {
      printf("%d",dungeon[i][j]);
    }
    printf("\n");
  }
}



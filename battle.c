
#include <stdlib.h>

#include "types.c"

#ifndef __DUNGEON_BATTLE__
#define __DUNGEON_BATTLE__

void monsterAttack() {
  int damage = rand()%(monsters[monster.id].damage[0]-monsters[monster.id].damage[1])+monsters[monster.id].damage[0];
  damage    -= player.def;
  if (damage <=0) damage = 1;
  player.pv -= damage;
  printf("receive %d damage\n", damage);
}

void playerLoose() {
  printf("you lose\n");
  gameMode = quit;
}

void playerWin() {
  int gain = monsters[monster.id].xp + monster.id - player.lvl +1;
  if (gain <= 0) gain = 1;
  printf("you win, gain %dxp\n", gain);
  player.xp += gain;
  int need = (int)pow(1.5, player.lvl - 1) * 10;
  if( player.xp >= need ) {
    printf("Gain one lvl \n");
    player.lvl ++;
    player.mpv = player.mpv + 5;
    player.pv  = player.mpv;
    player.fo  ++;
    player.def ++;
    player.xp  -= need;
  }
  gameMode = explorate;
}

void playerAttack() {
  int damage = player.fo;
  if (rand()%100<=10) {
    damage *= 1.8;
  }
  monster.pv -= damage;
  printf("deal %d damage\n", damage);
}

void battlePoolEvent(SDL_Event event) {
  switch(event.type) {
    case SDL_KEYDOWN :
      if (event.key.keysym.sym == SDLK_b) {
        playerAttack();
        if (monster.pv <=0) {
          playerWin();
        } else {
          monsterAttack();
          if (player.pv <= 0) {
            playerLoose();
          }
        }
      } else if (event.key.keysym.sym == SDLK_f) {
        monsterAttack();
        if (player.pv <= 0) {
          playerLoose();
        } else {
          printf("You fee\n");
          gameMode = explorate;
        }
      }
    break;
  }
}

void engageBattle() {
  int id = rand()%6;
  if (id<3)      id = player.stair;
  else if (id<5) id = player.stair+1;
  else           id = player.stair+2;
  gameMode       = battle;
  monster.pv     = monsters[id].pv;
  monster.id     = id;
}

#endif // __DUNGEON_BATTLE__


#include "types.c"

#ifndef __DUNGEON_BATTLE__
#define __DUNGEON_BATTLE__

void monsterAttack() {
  int damage = rand()%(monsters[monster.id].damage[0]-monsters[monster.id].damage[1])+monsters[monster.id].damage[0];
  player.pv -= damage;
  printf("receive %d damage\n", damage);
}

void playerAttack() {
  monster.pv -= player.f;
  printf("deal %d damage\n", player.f);
  if (monster.pv <=0) {
    printf("you win\n");
    gameMode = explorate;
  } else {
    monsterAttack();
  }
}

void battlePoolEvent(SDL_Event event) {
  switch(event.type) {
    case SDL_KEYDOWN :
      if (event.key.keysym.sym == SDLK_b) {
        playerAttack();
      } else if (event.key.keysym.sym == SDLK_f) {
        monsterAttack();
        printf("You fee\n");
        gameMode = explorate;
      }
    break;
  }
}

void engageBattle() {
  gameMode = battle;
  monster.pv     = monsters[0].pv;
  monster.id     = 0;
}

#endif // __DUNGEON_BATTLE__


void display3dView() {
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
  SDL_BlitSurface(groundTexts[0], NULL, screen, &groundRects[0]);
  SDL_BlitSurface(groundTexts[1], NULL, screen, &groundRects[1]);

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

void displayMinimap() {
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
  box.x = 224 / 2 - 1;
  box.y = 136 / 2 - 1;
  box.w = box.h = 3;
  SDL_FillRect(screen,&box,SDL_MapRGB(screen->format,0,0,192));
}

void exploratePollEvent(SDL_Event event) {
  switch(event.type) {
    case SDL_KEYDOWN :
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
      if (event.key.keysym.sym == SDLK_LEFT ) player.d = (player.d +3) % 4;
      break;
  }
}


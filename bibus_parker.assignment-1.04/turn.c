#include <stdlib.h>

#include "heap.h"
#include "dungeon.h"
#include "turn.h"
#include "utils.h"

static int32_t turn_cmp(const void *key, const void *with)
{
  uint32_t turn_stat = ((int32_t)((monster_t *)key)->next_turn -
                        (int32_t)((monster_t *)with)->next_turn);
  if (!turn_stat)
  {
    return ((int32_t)((monster_t *)key)->breaker -
            (int32_t)((monster_t *)with)->breaker);
  }
  else
  {
    return turn_stat;
  }
}

void turn_init(dungeon_t *d, heap_t *h)
{
  int counter;
  heap_init(h, turn_cmp, NULL);
  for (counter = 0; counter < d->num_monsters; counter++)
  {
    d->monsters[counter].hn = heap_insert(h, &(d->monsters[counter]));
  }
  d->pc.hn = heap_insert(h, &(d->pc.hn));
}

void kill_spot(dungeon_t *d, monster_t *mon, uint8_t pos_x, uint8_t pos_y){
    if(d->char_pos[pos_y][pos_x] != NULL && d->char_pos[pos_y][pos_x]->breaker != mon->breaker){
      d->char_pos[pos_y][pos_x]->alive = 0;
    }
}

void attempt_move(dungeon_t *d, monster_t *mon, uint8_t pos_x, uint8_t pos_y)
{
  if (hardnessxy(pos_x, pos_y) == 0)
  {
    kill_spot(d, mon, pos_x, pos_y);
    d->char_pos[mon->position[dim_y]][mon->position[dim_x]] = NULL;
    mon->position[dim_x] = pos_x;
    mon->position[dim_y] = pos_y;
    d->char_pos[mon->position[dim_y]][mon->position[dim_x]] = mon;
  }
  else if (mon->traits & trait_tunnel && hardnessxy(pos_x, pos_y) != 255)
  {
    if(hardnessxy(pos_x, pos_y) < 85){
      kill_spot(d, mon, pos_x, pos_y);
      hardnessxy(pos_x, pos_y) = 0;
      mapxy(pos_x, pos_y) = ter_floor_hall;
      d->char_pos[mon->position[dim_y]][mon->position[dim_x]] = NULL;
      mon->position[dim_x] = pos_x;
      mon->position[dim_y] = pos_y;
      d->char_pos[mon->position[dim_y]][mon->position[dim_x]] = mon;
    }else{
      hardnessxy(pos_x, pos_y) = hardnessxy(pos_x, pos_y) - 85;
    }
  }
}

//Does a turn and returns true if it was the PC's turn.
int do_turn(dungeon_t *d, heap_t *h)
{
  static monster_t *mon;
  //int goal_x, goal_y;
  mon = heap_remove_min(h);

  if(mon->alive){
  //Do the movement stuff and killing of monsters
  switch (mon->traits)
  {
  case 0:
    break;

  case trait_int:
    break;

  case trait_tele:
    break;

  case trait_tunnel:
    break;

  case trait_erratic:
    break;

  case trait_int|trait_tele:
    break;

  case trait_int|trait_tunnel:
    break;

  case trait_int|trait_erratic:
    break;

  case trait_tele|trait_tunnel:
    break;

  case trait_tele|trait_erratic:
    break;

  case trait_tunnel|trait_erratic:
    break;

  case trait_int|trait_tele|trait_tunnel:
    break;

  case trait_int|trait_tele|trait_erratic:
    break;

  case trait_int|trait_tunnel|trait_erratic:
    break;

  case trait_tele|trait_tunnel|trait_erratic:
    break;

  case trait_int|trait_tele|trait_tunnel|trait_erratic:
    break;
  }

  attempt_move(d, mon, mon->position[dim_x] + 1, mon->position[dim_y] + 1);

  // if(!mon->is_player){

  // }else{

  // }

  mon->next_turn = mon->next_turn + (1000 / (mon->speed));
  mon->hn = heap_insert(h, mon);
  }
  return mon->is_player;
}
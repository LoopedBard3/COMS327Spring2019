#ifndef TURN_H
#define TURN_H

// static int32_t turn_cmp(const void *key, const void *with);
void turn_init(dungeon_t *d, heap_t *h);
void kill_spot(dungeon_t *d, monster_t *mon, uint8_t pos_x, uint8_t pos_y);
void attempt_move(dungeon_t *d, monster_t *mon, uint8_t pos_x, uint8_t pos_y);
int do_turn(dungeon_t *d, heap_t *h);
int sees_player(dungeon_t *d, monster_t *mon);
void get_basic_move(dungeon_t *d, monster_t *mon, pair_t *change);
void get_random_move(pair_t* change);
void get_advanced_move(dungeon_t *d, monster_t *mon, pair_t *change);

#endif
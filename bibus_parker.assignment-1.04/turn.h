#ifndef TURN_H
#define TURN_H

// static int32_t turn_cmp(const void *key, const void *with);
void turn_init(dungeon_t *d, heap_t *h);
void kill_spot(dungeon_t *d, monster_t *mon, uint8_t pos_x, uint8_t pos_y);
void attempt_move(dungeon_t *d, monster_t *mon, uint8_t pos_x, uint8_t pos_y);
int do_turn(dungeon_t *d, heap_t *h);

#endif
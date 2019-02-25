#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "heap.h"
#include "dims.h"
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

int sees_player(dungeon_t *d, monster_t *mon)
{
    pair_t point1, point2;
    int new, error, x_hold, y_hold;
    //Ensure we draw left to right
    if (mon->position[dim_x] < d->pc.position[dim_x])
    {
        point1[dim_x] = mon->position[dim_x];
        point1[dim_y] = mon->position[dim_y];
        point2[dim_x] = d->pc.position[dim_x];
        point2[dim_y] = d->pc.position[dim_y];
    }
    else
    {
        point1[dim_x] = d->pc.position[dim_x];
        point1[dim_y] = d->pc.position[dim_y];
        point2[dim_x] = mon->position[dim_x];
        point2[dim_y] = mon->position[dim_y];
    }

    new = 2 * (point2[dim_y] - point1[dim_y]);
    error = new - (point2[dim_x] - point1[dim_x]);

    for (x_hold = 0, y_hold = 0; x_hold <= point2[dim_x]; x_hold++)
    {
        //Increment y once the slope error is enough
        if (error >= 0)
        {
            y_hold++;
            error -= 2 * (point2[dim_x] - point1[dim_x]);
        }
        if (hardnessxy(x_hold, y_hold) != 0)
        {
            return 0;
        }
        //Add slope error
        error += new;
    }
    return 1;
}

void get_basic_move(dungeon_t *d, monster_t *mon, pair_t *change)
{
    if (mon->move_goal[dim_x] - mon->position[dim_x] == 0)
    {
        (*change)[dim_x] = 0;
    }
    else if (mon->move_goal[dim_x] - mon->position[dim_x] < 0)
    {
        (*change)[dim_x] = -1;
    }
    else
    {
        (*change)[dim_x] = 1;
    }
    if (mon->move_goal[dim_y] - mon->position[dim_y] == 0)
    {
        (*change)[dim_y] = 0;
    }
    else if (mon->move_goal[dim_y] - mon->position[dim_y] < 0)
    {
        (*change)[dim_y] = -1;
    }
    else
    {
        (*change)[dim_y] = 1;
    }
}

void get_advanced_move(dungeon_t *d, monster_t *mon, pair_t *change)
{
    int x_hold, y_hold, x_low = -1, y_low = -1;
    int mon_x, mon_y;
    mon_x = mon->position[dim_x];
    mon_y = mon->position[dim_y];
    if (mon->traits & (trait_tunnel | trait_int))
    {
        for (x_hold = -1; x_hold < 2; x_hold++)
        {
            for (y_hold = -1; y_hold < 2; y_hold++)
            {
                if (d->pc_distance[mon_y + y_hold][mon_x + x_hold] < d->pc_distance[mon_y + y_low][mon_x + x_low])
                {
                    x_low = x_hold;
                    y_low = y_hold;
                }
            }
        }
        (*change)[dim_x] = x_low;
        (*change)[dim_y] = y_low;
    }
    else if (mon->traits & trait_int)
    {
        for (x_hold = -1; x_hold < 2; x_hold++)
        {
            for (y_hold = -1; y_hold < 2; y_hold++)
            {
                if (d->pc_tunnel[mon_y + y_hold][mon_x + x_hold] < d->pc_tunnel[mon_y + y_low][mon_x + x_low])
                {
                    x_low = x_hold;
                    y_low = y_hold;
                }
            }
        }
        (*change)[dim_x] = x_low;
        (*change)[dim_y] = y_low;
    }
    else
    {
        get_basic_move(d, mon, change);
    }
}

void get_random_move(pair_t *change)
{
    (*change)[dim_x] = rand() % 3 - 1;
    (*change)[dim_y] = rand() % 3 - 1;
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

void kill_spot(dungeon_t *d, monster_t *mon, uint8_t pos_x, uint8_t pos_y)
{
    if (d->char_pos[pos_y][pos_x] != NULL && d->char_pos[pos_y][pos_x]->breaker != mon->breaker)
    {
        d->char_pos[pos_y][pos_x]->alive = 0;
    }
}

void attempt_move(dungeon_t *d, monster_t *mon, uint8_t pos_x, uint8_t pos_y)
{
    //Check if valid, if not, turn gets skipped because something went wrong.
    if (!(pos_x - (mon->position[dim_x] - 1) <= 2) || !(pos_y - (mon->position[dim_y] - 1) <= 2))
    {
        return;
    }
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
        if (hardnessxy(pos_x, pos_y) < 85)
        {
            kill_spot(d, mon, pos_x, pos_y);
            hardnessxy(pos_x, pos_y) = 0;
            mapxy(pos_x, pos_y) = ter_floor_hall;
            d->char_pos[mon->position[dim_y]][mon->position[dim_x]] = NULL;
            mon->position[dim_x] = pos_x;
            mon->position[dim_y] = pos_y;
            d->char_pos[mon->position[dim_y]][mon->position[dim_x]] = mon;
        }
        else
        {
            hardnessxy(pos_x, pos_y) = hardnessxy(pos_x, pos_y) - 85;
        }
    }
}

//Does a turn and returns true if it was the PC's turn.
int do_turn(dungeon_t *d, heap_t *h)
{
    static monster_t *mon;
    pair_t change;
    mon = heap_remove_min(h);

    if (mon->alive)
    {
        change[dim_x] = 0;
        change[dim_y] = 0;
        if (mon->is_player)
        {
            change[dim_x] = rand() % 2;
            change[dim_y] = rand() % 2;
            int rad = 3, x_scan, y_scan;
            for (x_scan = rad * -1; x_scan <= rad; x_scan++)
            {
                for (y_scan = rad * -1; y_scan <= rad; y_scan++)
                {
                    //printf("X: %d, Y: %d\n", d->pc.position[dim_x] + x_scan, d->pc.position[dim_y] + y_scan);
                    if (d->pc.position[dim_y] + y_scan > 0 && d->pc.position[dim_y] + y_scan < DUNGEON_Y && d->pc.position[dim_x] + x_scan > 0 && d->pc.position[dim_x] + x_scan < DUNGEON_X && d->char_pos[d->pc.position[dim_y] + y_scan][d->pc.position[dim_x] + x_scan] != 0 && (x_scan != 0 || y_scan != 0))
                    {
                        change[dim_x] = (x_scan < 0) - (x_scan  > 0);
                        change[dim_y] = (y_scan < 0) - (y_scan  > 0);
                        //printf("Found\n");
                    }
                }
            }
            //printf("END PX: %d, PY: %d, CX: %d, CY: %d\n", d->pc.position[dim_x], d->pc.position[dim_y], change[dim_x], change[dim_y]);
        }
        else
        {
            if (mon->traits & trait_int && mon->traits & trait_tele)
            {
                mon->move_goal[dim_x] = d->pc.position[dim_x];
                mon->move_goal[dim_y] = d->pc.position[dim_y];
                get_advanced_move(d, mon, &change);
                // printf("%x, C3\n", mon->traits);
            }
            else if (mon->traits & trait_int)
            {
                if (sees_player(d, mon))
                {
                    mon->move_goal[dim_x] = d->pc.position[dim_x];
                    mon->move_goal[dim_y] = d->pc.position[dim_y];
                    get_advanced_move(d, mon, &change);
                }
                get_basic_move(d, mon, &change);
                // printf("%x, C1\n", mon->traits);
            }
            else if (mon->traits & trait_tele)
            {
                mon->move_goal[dim_x] = d->pc.position[dim_x];
                mon->move_goal[dim_y] = d->pc.position[dim_y];
                get_basic_move(d, mon, &change);
                // printf("%x, C2\n", mon->traits);
            }
            else
            {
                if (sees_player(d, mon))
                {
                    mon->move_goal[dim_x] = d->pc.position[dim_x];
                    mon->move_goal[dim_y] = d->pc.position[dim_y];
                    get_basic_move(d, mon, &change);
                }
                //printf("%x, C0\n", mon->traits);
            }

            if (mon->traits & trait_erratic && rand() & 0x1)
            {
                get_random_move(&change);
            }
        }

        attempt_move(d, mon, mon->position[dim_x] + change[dim_x], mon->position[dim_y] + change[dim_y]);
        mon->next_turn = mon->next_turn + (1000 / (mon->speed));
        mon->hn = heap_insert(h, mon);
    }
    return mon->is_player;
}
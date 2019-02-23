#include <stdio.h>
#include <stdlib.h>

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

void get_random_move(pair_t* change){
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
        if (mon->is_player)
        {
            change[dim_x] = 1;
            change[dim_y] = 0;
        }
        else
        {
            change[dim_x] = 1;
            change[dim_y] = 1;
            //Do the movement stuff and killing of monsters
            switch (mon->traits)
            {
            case 0:
            case trait_tunnel:
                if (sees_player(d, mon))
                {
                    mon->move_goal[dim_x] = d->pc.position[dim_x];
                    mon->move_goal[dim_y] = d->pc.position[dim_y];
                    get_basic_move(d, mon, &change);
                }
                break;

            case trait_int:
                if (sees_player(d, mon))
                {
                    mon->move_goal[dim_x] = d->pc.position[dim_x];
                    mon->move_goal[dim_y] = d->pc.position[dim_y];
                }
                //get_basic_move(d, mon, &change);
                break;

            case trait_tele:
                mon->move_goal[dim_x] = d->pc.position[dim_x];
                mon->move_goal[dim_y] = d->pc.position[dim_y];
                get_basic_move(d, mon, &change);
                break;

            case trait_erratic:
                if (sees_player(d, mon))
                {
                    mon->move_goal[dim_x] = d->pc.position[dim_x];
                    mon->move_goal[dim_y] = d->pc.position[dim_y];
                    get_basic_move(d, mon, &change);
                }
                if(rand() & 0x1){
                    get_random_move(&change);
                }
                break;

            case trait_int | trait_tele:
                break;

            case trait_int | trait_tunnel:
                break;

            case trait_int | trait_erratic:

                if(rand() & 0x1){
                    get_random_move(&change);
                }
                break;

            case trait_tele | trait_tunnel:
                break;

            case trait_tele | trait_erratic:
                break;

            case trait_tunnel | trait_erratic:

                if(rand() & 0x1){
                    get_random_move(&change);
                }
                break;

            case trait_int | trait_tele | trait_tunnel:
                break;

            case trait_int | trait_tele | trait_erratic:

                if(rand() & 0x1){
                    get_random_move(&change);
                }
                break;

            case trait_int | trait_tunnel | trait_erratic:

                if(rand() & 0x1){
                    get_random_move(&change);
                }
                break;

            case trait_tele | trait_tunnel | trait_erratic:

                if(rand() & 0x1){
                    get_random_move(&change);
                }
                break;

            case trait_int | trait_tele | trait_tunnel | trait_erratic:

                if(rand() & 0x1){
                    get_random_move(&change);
                }
                break;
            }
        }

        attempt_move(d, mon, mon->position[dim_x] + change[dim_x], mon->position[dim_y] + change[dim_y]);
        mon->next_turn = mon->next_turn + (1000 / (mon->speed));
        mon->hn = heap_insert(h, mon);
    }
    return mon->is_player;
}
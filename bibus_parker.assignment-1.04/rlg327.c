#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

#include "dungeon.h"
#include "path.h"

void usage(char *name)
{
  fprintf(stderr,
          "Usage: %s [-r|--rand <seed>] [-l|--load [<file>]]\n"
          "          [-s|--save [<file>]] [-i|--image <pgm file>]\n"
          "          [-n|--nummon <number of monsters>]\n",
          name);

  exit(-1);
}

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

static void turn_init(dungeon_t *d, heap_t *h)
{
  int counter;
  heap_init(h, turn_cmp, NULL);
  for (counter = 0; counter < d->num_monsters; counter++)
  {
    d->monsters[counter].hn = heap_insert(h, &(d->monsters[counter]));
  }
  d->pc.hn = heap_insert(h, &(d->pc.hn));
}

int space_valid(dungeon_t *d, uint8_t pos_x, uint8_t pos_y, char tunneling)
{
  if (hardnessxy(pos_x, pos_y) == 0)
  {
    return 1;
  }
  else if (tunneling && hardnessxy(pos_x, pos_y) != 255)
  {
    return 1;
  }
  return 0;
}

//Does a turn and returns true if it was the PC's turn.
static int do_turn(dungeon_t *d, heap_t *h)
{
  static monster_t *mon;
  //int goal_x, goal_y;
  mon = heap_remove_min(h);
  //mon = heap_peek_min(h);

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

  if (space_valid(d, mon->position[dim_x] + 1, mon->position[dim_y], mon->traits & trait_tunnel))
  {
    mon->position[dim_x] = mon->position[dim_x] + 1;
  }

  if (space_valid(d, mon->position[dim_x], mon->position[dim_y] + 1, mon->traits & trait_tunnel))
  {
    mon->position[dim_y] = mon->position[dim_y] + 1;
  }

  // if(space_valid(d, mon->position[dim_x], goal_y, mon->traits & trait_tunnel)){
  //   mon->position[dim_y] = goal_y;
  // }

  // if(!mon->is_player){

  // }else{

  // }

  mon->next_turn = mon->next_turn + (1000 / (mon->speed));
  printf("NT: %d\n", mon->next_turn);
  mon->hn = heap_insert(h, mon);
  //heap_decrease_key_no_replace(h, ((*mon).hn));
  return mon->is_player;
}

int main(int argc, char *argv[])
{
  dungeon_t d;
  heap_t heap;
  time_t seed;
  struct timeval tv;
  uint32_t i;
  uint32_t do_load, do_save, do_seed, do_image, do_save_seed, do_save_image;
  uint32_t long_arg;
  char *save_file;
  char *load_file;
  char *pgm_file;

  /* Quiet a false positive from valgrind. */
  memset(&d, 0, sizeof(d));

  /* Default behavior: Seed with the time, generate a new dungeon, *
   * and don't write to disk.                                      */
  do_load = do_save = do_image = do_save_seed = do_save_image = 0;
  do_seed = 1;
  d.num_monsters = 10; //Set the default number of monsters
  save_file = load_file = NULL;

  /* The project spec requires '--load' and '--save'.  It's common  *
   * to have short and long forms of most switches (assuming you    *
   * don't run out of letters).  For now, we've got plenty.  Long   *
   * forms use whole words and take two dashes.  Short forms use an *
    * abbreviation after a single dash.  We'll add '--rand' (to     *
   * specify a random seed), which will take an argument of it's    *
   * own, and we'll add short forms for all three commands, '-l',   *
   * '-s', and '-r', respectively.  We're also going to allow an    *
   * optional argument to load to allow us to load non-default save *
   * files.  No means to save to non-default locations, however.    *
   * And the final switch, '--image', allows me to create a dungeon *
   * from a PGM image, so that I was able to create those more      *
   * interesting test dungeons for you.                             */

  if (argc > 1)
  {
    for (i = 1, long_arg = 0; i < argc; i++, long_arg = 0)
    {
      if (argv[i][0] == '-')
      { /* All switches start with a dash */
        if (argv[i][1] == '-')
        {
          argv[i]++;    /* Make the argument have a single dash so we can */
          long_arg = 1; /* handle long and short args at the same place.  */
        }
        switch (argv[i][1])
        {
        case 'r':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-rand")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%lu", &seed) /* Argument is not an integer */)
          {
            usage(argv[0]);
          }
          do_seed = 0;
          break;
        case 'l':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-load")))
          {
            usage(argv[0]);
          }
          do_load = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-')
          {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            load_file = argv[++i];
          }
          break;
        case 's':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-save")))
          {
            usage(argv[0]);
          }
          do_save = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-')
          {
            /* There is another argument, and it's not a switch, so *
             * we'll save to it.  If it is "seed", we'll save to    *
	     * <the current seed>.rlg327.  If it is "image", we'll  *
	     * save to <the current image>.rlg327.                  */
            if (!strcmp(argv[++i], "seed"))
            {
              do_save_seed = 1;
              do_save_image = 0;
            }
            else if (!strcmp(argv[i], "image"))
            {
              do_save_image = 1;
              do_save_seed = 0;
            }
            else
            {
              save_file = argv[i];
            }
          }
          break;
        case 'i':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-image")))
          {
            usage(argv[0]);
          }
          do_image = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-')
          {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            pgm_file = argv[++i];
          }
          break;

        case 'n':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-nummon")))
          {
            usage(argv[0]);
          }
          if ((argc > i + 1) && argv[i + 1][0] != '-')
          {
            /* There is another argument, and it's not a switch, so *
             * we use it to set the number of monsters.    */
            d.num_monsters = atoi(argv[++i]);
          }
          break;
        default:
          usage(argv[0]);
        }
      }
      else
      { /* No dash */
        usage(argv[0]);
      }
    }
  }

  if (do_seed)
  {
    /* Allows me to generate more than one dungeon *
     * per second, as opposed to time().           */
    gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
  }

  printf("Seed is %ld.\n", seed);
  srand(seed);

  init_dungeon(&d);

  if (do_load)
  {
    read_dungeon(&d, load_file);
  }
  else if (do_image)
  {
    read_pgm(&d, pgm_file);
  }
  else
  {
    gen_dungeon(&d);
  }

  if (!do_load)
  {
    /* Set a valid position for the PC */
    d.pc.position[dim_x] = (d.rooms[0].position[dim_x] +
                            (rand() % d.rooms[0].size[dim_x]));
    d.pc.position[dim_y] = (d.rooms[0].position[dim_y] +
                            (rand() % d.rooms[0].size[dim_y]));
  }
  d.pc.is_player = 1;
  d.pc.breaker = 0;
  d.pc.speed = 10;
  gen_monsters(&d);
  turn_init(&d, &heap);
  printf("PC is at (y, x): %d, %d\n",
         d.pc.position[dim_y], d.pc.position[dim_x]);

  //Do the movement and monster turn code.
  while (d.pc.alive)
  {
    //if (do_turn(&d, &heap))
    {
      do_turn(&d, &heap);
      render_dungeon(&d);
      dijkstra(&d);
      dijkstra_tunnel(&d);
      usleep(250000);
    }
  }

  printf("You have perished a very untimely death!");

  if (do_save)
  {
    if (do_save_seed)
    {
      /* 10 bytes for number, plus dot, extention and null terminator. */
      save_file = malloc(18);
      sprintf(save_file, "%ld.rlg327", seed);
    }
    if (do_save_image)
    {
      if (!pgm_file)
      {
        fprintf(stderr, "No image file was loaded.  Using default.\n");
        do_save_image = 0;
      }
      else
      {
        /* Extension of 3 characters longer than image extension + null. */
        save_file = malloc(strlen(pgm_file) + 4);
        strcpy(save_file, pgm_file);
        strcpy(strchr(save_file, '.') + 1, "rlg327");
      }
    }
    write_dungeon(&d, save_file);

    if (do_save_seed || do_save_image)
    {
      free(save_file);
    }
  }

  delete_dungeon(&d);

  return 0;
}
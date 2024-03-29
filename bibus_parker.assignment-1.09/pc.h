#ifndef PC_H
# define PC_H

# include <stdint.h>

# include "dims.h"
# include "character.h"
# include "object.h" 
# include "dungeon.h"


typedef enum equipable_location {
  location_WEAPON,
  location_OFFHAND,
  location_RANGED,
  location_ARMOR,
  location_HELMET,
  location_CLOAK,
  location_GLOVES,
  location_BOOTS,
  location_AMULET,
  location_LIGHT,
  location_LRING,
  location_RRING,
  TOTAL_EQUIPS
} equipable_location_t;

class pc : public character {
 public:
  pc() {
    int counter;
    for(counter = 0; counter < PC_BACKPACKSIZE; counter++) backpack[counter] = NULL;
    for(counter = 0; counter < TOTAL_EQUIPS; counter++) equipped_items[counter] = NULL;
  }
  ~pc() {
    int counter;
    for(counter = 0; counter < PC_BACKPACKSIZE; counter++){
      if(backpack[counter] != NULL){
        delete backpack[counter];
        backpack[counter] = NULL;
      }
    } 
    for(counter = 0; counter < TOTAL_EQUIPS; counter++){
      if(equipped_items[counter] != NULL){
        delete equipped_items[counter];
        equipped_items[counter] = NULL;
      }
    } 
  }
  terrain_type known_terrain[DUNGEON_Y][DUNGEON_X];
  uint8_t visible[DUNGEON_Y][DUNGEON_X];
  object* backpack[PC_BACKPACKSIZE];
  object* equipped_items[TOTAL_EQUIPS];

  int pickup_object(dungeon *d);
  int equip_object(int item_pos);
  int unequip_object(int item_pos);
  int update_speed();
  int get_atk_damage();

  //Special Equip Cases
  int equip_ring(int item_pos);
};

void pc_delete(pc *pc);
uint32_t pc_is_alive(dungeon *d);
void config_pc(dungeon *d);
uint32_t pc_next_pos(dungeon *d, pair_t dir);
void place_pc(dungeon *d);
uint32_t pc_in_room(dungeon *d, uint32_t room);
void pc_learn_terrain(pc *p, pair_t pos, terrain_type ter);
terrain_type pc_learned_terrain(pc *p, int16_t y, int16_t x);
void pc_init_known_terrain(pc *p);
void pc_observe_terrain(pc *p, dungeon *d);
int32_t is_illuminated(pc *p, int16_t y, int16_t x);
void pc_reset_visibility(pc *p);
const char * get_equipable_loc_str(int pos);

#endif

#ifndef MDEF_H
# define MDEF_H

# include <stdint.h>
#include <string>

# include "dims.h"
# include "character.h"

# define NPC_SMART         0x00000001
# define NPC_TELEPATH      0x00000002
# define NPC_TUNNEL        0x00000004
# define NPC_ERRATIC       0x00000008
// # define NPC_BIT04         0x00000010
// # define NPC_BIT05         0x00000020
// # define NPC_BIT06         0x00000040
// # define NPC_BIT07         0x00000080
// # define NPC_BIT08         0x00000100
// # define NPC_BIT09         0x00000200
// # define NPC_BIT10         0x00000400
// # define NPC_BIT11         0x00000800
// # define NPC_BIT12         0x00001000
// # define NPC_BIT13         0x00002000
// # define NPC_BIT14         0x00004000
// # define NPC_BIT15         0x00008000
// # define NPC_BIT16         0x00010000
// # define NPC_BIT17         0x00020000
// # define NPC_BIT18         0x00040000
// # define NPC_BIT19         0x00080000
// # define NPC_BIT20         0x00100000
// # define NPC_BIT21         0x00200000
// # define NPC_BIT22         0x00400000
// # define NPC_BIT23         0x00800000
// # define NPC_BIT24         0x01000000
// # define NPC_BIT25         0x02000000
// # define NPC_BIT26         0x04000000
// # define NPC_BIT27         0x08000000
// # define NPC_BIT28         0x10000000
// # define NPC_BIT29         0x20000000
// # define NPC_BIT30         0x40000000
// # define NPC_BIT31         0x80000000

// # define has_characteristic(character, bit)              
//   ((character)->npc->characteristics & NPC_##bit)

enum Color { BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, NUMCOLORS };
enum CHECKED_FIELDS {NAME, SYMBOL, DESC, COLOR, SPEED, ABIL, HEALTH, ATK, RARITY, NUM_CHECK_FIELDS};

typedef uint32_t npc_characteristics_t;

class monster_definition {
 public:
  std::string name;
  char symbol;
  std::string description;
  int colors_selection[NUMCOLORS]{ 0 }; //Change to Enum
  int32_t speed_base;
  int32_t speed_dice;
  int32_t speed_sides;
  npc_characteristics_t abilities;
  int32_t hp_base;
  int32_t hp_dice;
  int32_t hp_sides;
  int32_t atk_base;
  int32_t atk_dice;
  int32_t atk_sides;
  int32_t rarity;
};

class monster_def_parser {
    public:
    int fields_checked[NUM_CHECK_FIELDS];
    monster_definition curr_monster;
    monster_definition* monster_def_list;
    void parseWOutput();
    void empty();
    void reset_current();
};

#endif

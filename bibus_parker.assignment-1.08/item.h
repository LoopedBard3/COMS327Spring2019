#ifndef ITEM_H
# define ITEM_H

# include <stdint.h>
# include <string>
# include "dice.h"

typedef enum object_type {
  objtype_no_type,
  objtype_WEAPON,
  objtype_OFFHAND,
  objtype_RANGED,
  objtype_LIGHT,
  objtype_ARMOR,
  objtype_HELMET,
  objtype_CLOAK,
  objtype_GLOVES,
  objtype_BOOTS,
  objtype_AMULET,
  objtype_RING,
  objtype_SCROLL,
  objtype_BOOK,
  objtype_FLASK,
  objtype_GOLD,
  objtype_AMMUNITION,
  objtype_FOOD,
  objtype_WAND,
  objtype_CONTAINER
} object_type_t;


class item {
 private:
  std::string name, description;
  object_type_t type;
  uint32_t color, hit, dodge, defence, weight, speed, attribute, value;
  dice damage;
 public:
  item() : name(), description(), type(objtype_no_type),
           color(0),  hit(),  dodge(),   
           defence(),  weight(), speed(),   
           attribute(),  value(),  damage()
  {
  }

  /* Need all these accessors because otherwise there is a *
   * circular dependancy that is difficult to get around.  */
  inline const std::string &get_name() const { return name; }
  inline const std::string &get_description() const { return description; }
  inline const object_type_t get_type() const { return type; }
  inline const uint32_t get_color() const { return color; }
  inline const uint32_t &get_hit() const { return hit; }
  inline const dice &get_damage() const { return damage; }
  inline const uint32_t &get_dodge() const { return dodge; }
  inline const uint32_t &get_defence() const { return defence; }
  inline const uint32_t &get_weight() const { return weight; }
  inline const uint32_t &get_speed() const { return speed; }
  inline const uint32_t &get_attribute() const { return attribute; }
  inline const uint32_t &get_value() const { return value; }
};


#endif
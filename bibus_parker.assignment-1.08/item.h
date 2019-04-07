#ifndef ITEM_H
# define ITEM_H

# include <stdint.h>
# include <string>
# include "dice.h"
# include "dims.h"
# include "object_type.h"

class item {
 public:
  std::string name, description;
  object_type_t type;
  uint32_t color, hit, dodge, defence, weight, speed, attribute, value;
  dice damage;
  char symbol;
  pair_t position;
 public:
  item() : name(), description(), type(objtype_no_type),
           color(0),  hit(),  dodge(),   
           defence(),  weight(), speed(),   
           attribute(),  value(),  damage(), symbol(),
           position()
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
  inline const char &get_symbol() const { return symbol; }
};

int16_t *item_get_pos(item *it);
char item_get_symbol(const item *it);

#endif
# include "item.h"


int16_t *item_get_pos(item *it)
{
  return it->position;
}

char item_get_symbol(const item *it)
{
  return it->symbol;
}

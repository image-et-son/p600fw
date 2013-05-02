#ifndef POTMUX_H
#define	POTMUX_H

#include "p600.h"

uint16_t potmux_getValue(p600Pot_t pot);
p600Pot_t potmux_detectChange(void);

void potmux_init(void);
void potmux_update(int8_t updateRegular, int8_t updatePriority);

#endif	/* POTMUX_H */


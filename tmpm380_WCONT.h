#ifndef __TMPM380_WCONT_H
#define __TMPM380_WCONT_H

#ifndef __MAIN_H
#include "main.h"
#endif

void WCONT(void);
void WCSETUP(uint8_t MX0, uint8_t WLO);
void WCRUN(void);
void WCLEAN(void);

#endif
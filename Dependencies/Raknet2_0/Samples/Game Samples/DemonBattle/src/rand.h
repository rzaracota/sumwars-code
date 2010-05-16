#ifndef __RAND_H
#define __RAND_H

extern void seedMT(unsigned long seed); // Defined in cokus_c.c
extern unsigned long reloadMT(void);
extern unsigned long randomMT(void);
extern float frandomMT(void);

#endif
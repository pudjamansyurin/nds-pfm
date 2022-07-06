/*
 * pfm.h
 *
 *  Created on: May 25, 2022
 *      Author: pujak
 */

#ifndef PFM_PFM_H_
#define PFM_PFM_H_

#include <stdint.h>

#define NDS32_PERFCTL_EN0   (1UL << 0)
#define NDS32_PERFCTL_EN1   (1UL << 1)
#define NDS32_PERFCTL_EN2   (1UL << 2)
#define NDS32_PERFCTL_IE0   (1UL << 3)
#define NDS32_PERFCTL_IE1   (1UL << 4)
#define NDS32_PERFCTL_IE2   (1UL << 5)
#define NDS32_PERFCTL_OVF0  (1UL << 6)
#define NDS32_PERFCTL_OVF1  (1UL << 7)
#define NDS32_PERFCTL_OVF2  (1UL << 8)
#define NDS32_PERFCTL_KS0   (1UL << 9)
#define NDS32_PERFCTL_KS1   (1UL << 10)
#define NDS32_PERFCTL_KS2   (1UL << 11)
#define NDS32_PERFCTL_KU0   (1UL << 12)
#define NDS32_PERFCTL_KU1   (1UL << 13)
#define NDS32_PERFCTL_KU2   (1UL << 14)
#define NDS32_PERFCTL_SEL0_C (0UL << 15)
#define NDS32_PERFCTL_SEL0_I (1UL << 15)
#define NDS32_PERFCTL_SEL1  (0x3f << 16)
#define NDS32_PERFCTL_SEL2  (0x3f << 22)

#define NDS32_ISET          (0x07 << 0)
#define NDS32_IWAY          (0x07 << 3)
#define NDS32_ISIZE         (0x07 << 6)
#define NDS32_DSET          (0x07 << 0)
#define NDS32_DWAY          (0x07 << 3)
#define NDS32_DSIZE         (0x07 << 6)

/* Public function declarations */
void pfm_start(void);
void pfm_stop(void);
unsigned int pfm_read(void);
void pfm_disableIDCache(void);
void pfm_enableIDCache(void);

#endif /* PFM_PFM_H_ */

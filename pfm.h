/*
 * pfm.h
 *
 *  Created on: May 25, 2022
 *      Author: pujak
 */

#ifndef PFM_PFM_H_
#define PFM_PFM_H_

#include <stdint.h>

/* Public function declarations */
void pfm_start(void);
void pfm_stop(void);
unsigned int pfm_read(void);
void pfm_disableIDCache(void);
void pfm_enableIDCache(void);

#endif /* PFM_PFM_H_ */

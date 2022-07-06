/*
 * pfm.c
 *
 *  Created on: May 25, 2022
 *      Author: pujak
 */

#include "pfm.h"
#include <stdio.h>
#include <nds32_intrinsic.h>

/* Public function definitions */
void pfm_start(void)
{
	unsigned int counter = 0, ctrl = 0, msc_cfg = 0;

	msc_cfg = __nds32__mfsr(NDS32_SR_MSC_CFG);
	if (!(msc_cfg & 0x4)) {
		printf("This CPU config doesn't have pfm to use.\n");
		return;
	}

	__nds32__mtsr_dsb(counter, NDS32_SR_PFMC0);

	/* Enable performance counter0 , Enable interrupt for counter0 */
	/* Calculate Cycles for counter0 */
	ctrl |= NDS32_PERFCTL_EN0 | NDS32_PERFCTL_IE0 | NDS32_PERFCTL_SEL0_C;
	__nds32__mtsr(ctrl, NDS32_SR_PFM_CTL);
}

void pfm_stop(void)
{
	unsigned int pfm_ctl, msc_cfg = 0;

	msc_cfg = __nds32__mfsr(NDS32_SR_MSC_CFG);
	if (!(msc_cfg & 0x4)) {
		printf("This CPU config doesn't have pfm to use.\n");
		return;
	}

	pfm_ctl = __nds32__mfsr(NDS32_SR_PFM_CTL);
	//get PFM_CTL
	pfm_ctl = pfm_ctl & 0xFFFFFFF8;
	__nds32__mtsr(pfm_ctl, NDS32_SR_PFM_CTL);

}

unsigned int pfm_read(void)
{
	unsigned int msc_cfg = 0, cycles;

	msc_cfg = __nds32__mfsr(NDS32_SR_MSC_CFG);
	if (!(msc_cfg & 0x4)) {
		printf("This CPU config doesn't have pfm to use.\n");
		return 0;
	}

	cycles = __nds32__mfsr(NDS32_SR_PFMC0);
	return cycles;
}

void pfm_disableIDCache(void)
{
#ifndef __NDS32_ISA_V3M__
	unsigned int dcm_cfg, cache_line, end;

	/* Check if cache available */
	if (!(__nds32__mfsr(NDS32_SR_ICM_CFG) & NDS32_ISIZE) ||
	    !(__nds32__mfsr(NDS32_SR_CACHE_CTL) & 0x3))
		return;

	/* DCache end = cache line size * cache set * cache way. */
	dcm_cfg = __nds32__mfsr(NDS32_SR_DCM_CFG);
	end = cache_line = 1 << (((dcm_cfg & NDS32_DSIZE) >> 6) + 2);
	end *= (1 << ((dcm_cfg & NDS32_DSET) + 6));
	end *= (((dcm_cfg & NDS32_DWAY) >> 3) + 1);

	/* Flush DCache */
	do {
		end -= cache_line;
		__nds32__cctlidx_wbinval(NDS32_CCTL_L1D_IX_WB, end);
		__nds32__dsb();
	} while (end > 0);

	/* Invalid DCache */
	__nds32__cctl_l1d_invalall();
	__nds32__dsb();

	/* Disable I/D Cache */
	__nds32__mtsr_dsb(__nds32__mfsr(NDS32_SR_CACHE_CTL) & ~0x3, NDS32_SR_CACHE_CTL);
#endif
}

void pfm_enableIDCache(void)
{
	unsigned int cache_ctl=0, mmu_ctl, icm_cfg, dcm_cfg;
	unsigned int iset, iway, isize;
	unsigned int dset, dway, dsize;

	/* Get ICache ways, sets, line size  */
	icm_cfg = __nds32__mfsr(NDS32_SR_ICM_CFG);
	if ((icm_cfg & NDS32_ISET) < 7)
		iset = 1 << ((icm_cfg & NDS32_ISET) + 6);
	else
		iset = 0;
	printf("The ICache sets = %u\n", iset);

	iway = ((icm_cfg & NDS32_IWAY) >> 3) + 1;
	printf("The ICache ways = %u\n", iway);

	if (((icm_cfg & NDS32_ISIZE) >> 6)
	    && ((icm_cfg & NDS32_ISIZE) >> 6) <= 5)
		isize = 1 << (((icm_cfg & NDS32_ISIZE) >> 6) + 2);
	else if (((icm_cfg & NDS32_ISIZE) >> 6) >= 6) {
		printf("Warning i cacheline size is reserved value\n");
		isize = 0;
	} else
		isize = 0;
	printf("The ICache line size = %u\n", isize);

	if (isize == 0)
		printf("This CPU doesn't have ICache.\n");
	else {
		cache_ctl = __nds32__mfsr(NDS32_SR_CACHE_CTL);
		cache_ctl |= 0x1;
		/* Enable I Cache */
		__nds32__mtsr(cache_ctl, NDS32_SR_CACHE_CTL);
	}
	/* Get DCache ways, sets, line size  */
	dcm_cfg = __nds32__mfsr(NDS32_SR_DCM_CFG);
	if ((dcm_cfg & NDS32_DSET) < 7)
		dset = 1 << ((dcm_cfg & NDS32_DSET) + 6);
	else
		dset = 0;
	printf("The DCache sets = %u\n", dset);

	dway = ((dcm_cfg & NDS32_DWAY) >> 3) + 1;
	printf("The DCache ways = %u\n", dway);

	if (((dcm_cfg & NDS32_DSIZE) >> 6)
	    && ((dcm_cfg & NDS32_DSIZE) >> 6) <= 5)
		dsize = 1 << (((dcm_cfg & NDS32_DSIZE) >> 6) + 2);
	else if (((dcm_cfg & NDS32_DSIZE) >> 6) >= 6) {
		printf("Warning d cacheline size is reserved value\n");
		dsize = 0;
	} else
		dsize = 0;
	printf("The DCache line size = %u\n", dsize);
	if (dsize == 0)
		printf("This CPU doesn't have DCache.\n");
	else {
		cache_ctl = __nds32__mfsr(NDS32_SR_CACHE_CTL);
		cache_ctl |= 0x2;
		/* Enable D Cache */
		__nds32__mtsr(cache_ctl, NDS32_SR_CACHE_CTL);
	}

	/* Cacheable/Write-Back for NTC0 */
	mmu_ctl = __nds32__mfsr(NDS32_SR_MMU_CTL);
	mmu_ctl |= 0x2 << 1;
	__nds32__mtsr_isb(mmu_ctl, NDS32_SR_MMU_CTL);
	if (cache_ctl & 0x1)
		printf("Enable I cache\n");
	if (cache_ctl & 0x2)
		printf("Enable D cache\n");
	if (!(cache_ctl & 0x3))
		printf("Can't enable I/D cache\n");
}

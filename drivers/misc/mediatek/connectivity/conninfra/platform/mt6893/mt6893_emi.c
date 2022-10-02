// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2020 MediaTek Inc.
 */

#define pr_fmt(fmt) KBUILD_MODNAME "@(%s:%d) " fmt, __func__, __LINE__

#include <linux/memblock.h>
#include <linux/platform_device.h>
#ifdef CONFIG_MTK_EMI
#include <mt_emi_api.h>
#endif
#include <linux/of_reserved_mem.h>

#include <memory/mediatek/emi.h>
#include "mt6893_emi.h"
#include "mt6893.h"
#include "mt6893_consys_reg.h"
#include "consys_hw.h"
#include "consys_reg_util.h"
#include "mt6893_pos.h"

/* For MCIF */
#include <mtk_ccci_common.h>

/*******************************************************************************
*                         C O M P I L E R   F L A G S
********************************************************************************
*/

/*******************************************************************************
*                                 M A C R O S
********************************************************************************
*/
#define	REGION_CONN	27

#define	DOMAIN_AP	0
#define	DOMAIN_CONN	2
#define	DOMAIN_SCP	3

/*******************************************************************************
*                    E X T E R N A L   R E F E R E N C E S
********************************************************************************
*/

/*******************************************************************************
*                              C O N S T A N T S
********************************************************************************
*/

/*******************************************************************************
*                             D A T A   T Y P E S
********************************************************************************
*/

/*******************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
********************************************************************************
*/
/*******************************************************************************
*                            P U B L I C   D A T A
********************************************************************************
*/

extern unsigned long long gConEmiSize;
extern phys_addr_t gConEmiPhyBase;

struct consys_platform_emi_ops g_consys_platform_emi_ops_mt6893 = {
	.consys_ic_emi_mpu_set_region_protection = consys_emi_mpu_set_region_protection,
	.consys_ic_emi_set_remapping_reg = consys_emi_set_remapping_reg_mt6893,
	.consys_ic_emi_get_md_shared_emi = consys_emi_get_md_shared_emi,
};

/*******************************************************************************
*                           P R I V A T E   D A T A
********************************************************************************
*/

/*******************************************************************************
*                              F U N C T I O N S
********************************************************************************
*/

int consys_emi_mpu_set_region_protection(void)
{
	struct emimpu_region_t region;
	unsigned long long start = gConEmiPhyBase;
	unsigned long long end = gConEmiPhyBase + gConEmiSize - 1;

	mtk_emimpu_init_region(&region, REGION_CONN);
	mtk_emimpu_set_addr(&region, start, end);
	mtk_emimpu_set_apc(&region, DOMAIN_AP, MTK_EMIMPU_NO_PROTECTION);
	mtk_emimpu_set_apc(&region, DOMAIN_CONN, MTK_EMIMPU_NO_PROTECTION);
	/* for scp */
	mtk_emimpu_set_apc(&region, DOMAIN_SCP, MTK_EMIMPU_NO_PROTECTION);
	mtk_emimpu_set_protection(&region);
	mtk_emimpu_free_region(&region);

	pr_info("setting MPU for EMI share memory\n");
	return 0;
}

void consys_emi_get_md_shared_emi(phys_addr_t* base, unsigned int* size)
{
	phys_addr_t mdPhy = 0;
	int ret = 0;

#ifdef CONFIG_MTK_ECCCI_DRIVER
	mdPhy = get_smem_phy_start_addr(MD_SYS1, SMEM_USER_RAW_MD_CONSYS, &ret);
	pr_info("MCIF base=0x%llx size=0x%x", mdPhy, ret);
#else
	pr_info("[%s] ECCCI Driver is not supported.\n", __func__);
#endif
	if (ret && mdPhy) {
		if (base)
			*base = mdPhy;
		if (size)
			*size = ret;
	} else {
		pr_info("MCIF is not supported");
		if (base)
			*base = 0;
		if (size)
			*size = 0;
	}
}


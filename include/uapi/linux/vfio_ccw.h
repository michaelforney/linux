/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/*
 * Interfaces for vfio-ccw
 *
 * Copyright IBM Corp. 2017
 *
 * Author(s): Dong Jia Shi <bjsdjshi@linux.vnet.ibm.com>
 */

#ifndef _VFIO_CCW_H_
#define _VFIO_CCW_H_

#include <linux/types.h>

/* used for START SUBCHANNEL, always present */
struct __packed ccw_io_region {
#define ORB_AREA_SIZE 12
	__u8	orb_area[ORB_AREA_SIZE];
#define SCSW_AREA_SIZE 12
	__u8	scsw_area[SCSW_AREA_SIZE];
#define IRB_AREA_SIZE 96
	__u8	irb_area[IRB_AREA_SIZE];
	__u32	ret_code;
};

/*
 * used for processing commands that trigger asynchronous actions
 * Note: this is controlled by a capability
 */
#define VFIO_CCW_ASYNC_CMD_HSCH (1 << 0)
#define VFIO_CCW_ASYNC_CMD_CSCH (1 << 1)
struct __packed ccw_cmd_region {
	__u32 command;
	__u32 ret_code;
};

/*
 * Used for processing commands that read the subchannel-information block
 * Reading this region triggers a stsch() to hardware
 * Note: this is controlled by a capability
 */
struct __packed ccw_schib_region {
#define SCHIB_AREA_SIZE 52
	__u8 schib_area[SCHIB_AREA_SIZE];
};

/*
 * Used for returning a Channel Report Word to userspace.
 * Note: this is controlled by a capability
 */
struct __packed ccw_crw_region {
	__u32 crw;
	__u32 pad;
};

#endif

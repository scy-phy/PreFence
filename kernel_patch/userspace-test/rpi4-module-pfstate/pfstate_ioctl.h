#pragma once

#include <linux/ioctl.h>

// major device number
#define MAJOR_NUM 104

// definition of sub-commands supported by the ioctl device
// IOCTL_QUERY: query the prefetcher's state. Parameter (int): CPU ID
#define IOCTL_QUERY _IOR(MAJOR_NUM, 0, int)

// name of the device file
#define DEVICE_FILE_NAME "pfstate_dev"

#define PFSTATE_DISABLE_LOAD_STORE_HW_PF (1ULL<<5)
#define PFSTATE_L1_L2_PREFETCH_CROSS_4KB (1ULL<<4)
#define PFSTATE_LOAD_STORE_HW_PF_USE_VA (1ULL<<3)
#define PFSTATE_DISABLE_PF_READUNIQUE (1ULL<<2)
#define PFSTATE_DISABLE_L1I_PREFETCH (1ULL<<1)
#define PFSTATE_DISABLE_L2_TLB_PREFETCH (1ULL<<0)

#define PFSTATE_STR_DISABLE_LOAD_STORE_HW_PF ("DISABLE_LOAD_STORE_HW_PF")
#define PFSTATE_STR_L1_L2_PREFETCH_CROSS_4KB ("L1_L2_PREFETCH_CROSS_4KB")
#define PFSTATE_STR_LOAD_STORE_HW_PF_USE_VA ("LOAD_STORE_HW_PF_USE_VA")
#define PFSTATE_STR_DISABLE_PF_READUNIQUE ("DISABLE_PF_READUNIQUE")
#define PFSTATE_STR_DISABLE_L1I_PREFETCH ("DISABLE_L1I_PREFETCH")
#define PFSTATE_STR_DISABLE_L2_TLB_PREFETCH ("DISABLE_L2_TLB_PREFETCH")
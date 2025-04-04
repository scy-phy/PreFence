#pragma once

#include <linux/ioctl.h>

// major device number
#define MAJOR_NUM 104

// definition of sub-commands supported by the ioctl device
#ifdef __x86_64__
	// ===== Intel =====
	#define IOCTL_CTRLPF_INTEL_MSRVALUE _IOR(MAJOR_NUM, 0, uint32_t)
	#define IOCTL_CTRLPF_INTEL_TOGGLE_MSRVALUE _IOR(MAJOR_NUM, 1, uint32_t)
#elif defined(__aarch64__) && ! defined(__APPLE__)
	// ===== ARM A72 =====
	#define IOCTL_CTRLPF_ARM_A72_ENABLE_ALL _IO(MAJOR_NUM, 2)
	#define IOCTL_CTRLPF_ARM_A72_DISABLE_ALL _IO(MAJOR_NUM, 3)
	#define IOCTL_CTRLPF_ARM_A72_TOGGLE_ALL _IO(MAJOR_NUM, 4)
#endif
#define IOCTL_CTRLPF_DUMMY_CALL _IO(MAJOR_NUM, 5)

// name of the device file
#define DEVICE_FILE_NAME "ctrlpf"
#define DEVICE_FILE_PATH "/dev/" DEVICE_FILE_NAME
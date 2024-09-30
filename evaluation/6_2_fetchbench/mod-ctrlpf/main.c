// ioctl parts based on https://tldp.org/LDP/lkmpg/2.4/html/x856.html

#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>

#include "ctrlpf_ioctl.h"		// ioctl-related definitions

// The name for the device, as it will appear in /proc/devices
#define DEVICE_NAME DEVICE_FILE_NAME

// Is the device open right now? Used to prevent concurent access into the same device.
static int device_open_ctr = 0;

// model-specific register (MSR) and flag definitions *********************

#ifdef __x86_64__
	// Intel MSR
	#define INTEL_MSR_MISC_FEATURE_CONTROL (0x1A4)
#elif defined(__aarch64__) && ! defined(__APPLE__)
	// ARM A72
	#define A72_REG_CPUACTLR_EL1 "S3_1_c15_c2_0"
	#define A72_FLAG_LOAD_STORE_HW_PF (1ULL<<56)
	#define A72_FLAG_L1_L2_PREFETCH_CROSS_4KB (1ULL<<47)
	#define A72_FLAG_LOAD_STORE_HW_PF_USE_VA (1ULL<<43)
	#define A72_FLAG_DISABLE_PF_READUNIQUE (1ULL<<42)
	#define A72_FLAG_DISABLE_L1I_PREFETCH (1ULL<<32)
	#define A72_FLAG_DISABLE_L2_TLB_PREFETCH (1ULL<<21)
	#define A72_DISABLE_ORMASK ( \
		A72_FLAG_LOAD_STORE_HW_PF | \
		A72_FLAG_L1_L2_PREFETCH_CROSS_4KB | \
		A72_FLAG_LOAD_STORE_HW_PF_USE_VA | \
		A72_FLAG_DISABLE_PF_READUNIQUE | \
		A72_FLAG_DISABLE_L1I_PREFETCH | \
		A72_FLAG_DISABLE_L2_TLB_PREFETCH \
	)
	#define A72_ENABLE_ANDMASK ( \
		~ ( \
			A72_FLAG_LOAD_STORE_HW_PF | \
			A72_FLAG_L1_L2_PREFETCH_CROSS_4KB | \
			A72_FLAG_LOAD_STORE_HW_PF_USE_VA | \
			A72_FLAG_DISABLE_PF_READUNIQUE | \
			A72_FLAG_DISABLE_L1I_PREFETCH | \
			A72_FLAG_DISABLE_L2_TLB_PREFETCH \
		) \
	)
#endif

// ioctl functions ***************************

/**
 * @brief      Called whenever a process attempts to open the device file.
 *             allow only 1 process to open the device at a time
 */
static int device_open(struct inode *inode, struct file *file) {
	if (device_open_ctr != 0) {
		return -EBUSY;
	}
	device_open_ctr++;
	return 0;
}

/**
 * @brief      Called when a process closes the device file.
 *
 */
static int device_release(struct inode *inode, struct file *file) {
	device_open_ctr--;	
	return 0;
}

/**
 * @brief      Called whenever a process tries to do an ioctl on our device file.
 *
 * @param      file         The device file
 * @param[in]  ioctl_num    The ioctl number called
 * @param[in]  ioctl_param  The ioctl parameter given to the ioctl function
 *
 * @return     0 on success, -1 otherwise
 */
long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param) {
	switch (ioctl_num) {
		#ifdef __x86_64__
			// ===== Intel =====

			case IOCTL_CTRLPF_INTEL_MSRVALUE: {
				uint32_t lo = (uint32_t)ioctl_param;
				asm volatile("wrmsr" : : "a"(lo), "d"(0) /* hi */, "c"(INTEL_MSR_MISC_FEATURE_CONTROL));
				break;
			}
			case IOCTL_CTRLPF_INTEL_TOGGLE_MSRVALUE: {
				uint32_t lo = (uint32_t)ioctl_param;
				// disable all
				asm volatile("wrmsr" : : "a"(0b1111), "d"(0) /* hi */, "c"(INTEL_MSR_MISC_FEATURE_CONTROL));
				// enable as specified by parameter
				asm volatile("wrmsr" : : "a"(lo), "d"(0) /* hi */, "c"(INTEL_MSR_MISC_FEATURE_CONTROL));
				break;
			}
		#elif defined(__aarch64__) && ! defined(__APPLE__)
			// ===== ARM A72 =====
			case IOCTL_CTRLPF_ARM_A72_DISABLE_ALL: {
				uint64_t state = 0;
				// read current state
				asm volatile("MRS %0, " A72_REG_CPUACTLR_EL1 : "=r" (state));
				// update state
				state |= A72_DISABLE_ORMASK;
				asm volatile("MSR " A72_REG_CPUACTLR_EL1 ", %0" : : "r" (state));
				break;
			}
			case IOCTL_CTRLPF_ARM_A72_ENABLE_ALL: {
				uint64_t state = 0;
				// read current state into state
				asm volatile("MRS %0, " A72_REG_CPUACTLR_EL1 : "=r" (state));
				// update state
				state &= A72_ENABLE_ANDMASK;
				asm volatile("MSR " A72_REG_CPUACTLR_EL1 ", %0" : : "r" (state));
				break;
			}
			case IOCTL_CTRLPF_ARM_A72_TOGGLE_ALL: {
				uint64_t state = 0;
				// read current state
				asm volatile("MRS %0, " A72_REG_CPUACTLR_EL1 : "=r" (state));
				// update state (disable)
				state |= A72_DISABLE_ORMASK;
				asm volatile("MSR " A72_REG_CPUACTLR_EL1 ", %0" : : "r" (state));
				// update state (enable)
				state &= A72_ENABLE_ANDMASK;
				asm volatile("MSR " A72_REG_CPUACTLR_EL1 ", %0" : : "r" (state));
				break;
			}
		#endif
		case IOCTL_CTRLPF_DUMMY_CALL: {
			// Perform some read/write ops
			uint64_t volatile dummy = 42;
			dummy += 1;
			break;
		}
	}
	return 0;
}

// Module functions ***************************

struct file_operations Fops = {
	.unlocked_ioctl = device_ioctl,
	.open = device_open,
	.release = device_release // close
};

/**
 * @brief      Initialize the module and register the character device
 */
int init_module() {
	int ret_val;

	// Register the character device (at least try)
	ret_val = register_chrdev(MAJOR_NUM, DEVICE_NAME, &Fops);

	// Negative values signify an error
	if (ret_val < 0) {
		printk(
			"Sorry, registering the character device failed with %d\n",
			ret_val
		);
		return ret_val;
	}

	printk(KERN_INFO "Registeration successful. The major device number is %d.\n", MAJOR_NUM);
	printk(
		KERN_INFO 
		"If you want to talk to the module, you'll have to create a device file.\n"
		"We suggest you use:\n"
	);
	printk(KERN_INFO "mknod %s c %d 0\n", DEVICE_FILE_NAME, MAJOR_NUM);

	return 0;
}

/**
 * @brief      Cleanup - unregister the appropriate file from /proc
 */
void cleanup_module() {
	// Unregister the device
	unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
	printk(KERN_INFO "Module unloaded\n");
}

MODULE_LICENSE("GPL");

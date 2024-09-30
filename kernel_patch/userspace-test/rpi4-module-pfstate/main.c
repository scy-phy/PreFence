// ioctl parts based on https://tldp.org/LDP/lkmpg/2.4/html/x856.html

#include <linux/module.h>
#include <linux/kernel.h>

#include "pfstate_ioctl.h"		// ioctl-related definitions

// The name for the device, as it will appear in /proc/devices
#define DEVICE_NAME DEVICE_FILE_NAME

// Is the device open right now? Used to prevent concurent access into the same device.
static int _device_open_ctr = 0;

#define CPUACTLR_EL1 "S3_1_c15_c2_0"

#define MSR_DISABLE_LOAD_STORE_HW_PF (1ULL<<56)
#define MSR_L1_L2_PREFETCH_CROSS_4KB (1ULL<<47)
#define MSR_LOAD_STORE_HW_PF_USE_VA (1ULL<<43)
#define MSR_DISABLE_PF_READUNIQUE (1ULL<<42)
#define MSR_DISABLE_L1I_PREFETCH (1ULL<<32)
#define MSR_DISABLE_L2_TLB_PREFETCH (1ULL<<21)

// ioctl functions ***************************

/**
 * @brief      Called whenever a process attempts to open the device file.
 *             allow only 1 process to open the device at a time
 */
static int device_open(struct inode *inode, struct file *file) {
	if (_device_open_ctr != 0) {
		return -EBUSY;
	}
	_device_open_ctr++;
	return 0;
}

/**
 * @brief      Called when a process closes the device file.
 *
 */
static int device_release(struct inode *inode, struct file *file) {
	_device_open_ctr--;
	return 0;
}

/**
 * @brief      Helper function to run the query on a specific CPU. The
 *             kernel does not provide a way to execute code on a given CPU
 *             (at least I did not find one). To work around this, this
 *             function is called on all CPUs, and the cache query is only
 *             executed when the function happens to run on the right one.
 *
 * @param      info  pointer to a size_t variable where the number of
 *                   results can be stored after successful execution.
 */

void query(void* info) {
	uint64_t requested_cpuno = *((uint64_t*)info);

	// get number of current cpu and disable preemption -> we stay on this
	// CPU until we release it with put_cpu().
	int cpuno = get_cpu();
	if (cpuno == requested_cpuno) {
		// run query (only on the specified CPU)
		uint64_t msr = 0;
		asm volatile("MRS %0, " CPUACTLR_EL1 : "=r" (msr));
		*((uint64_t*)info) = msr;
	}
	put_cpu();
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
		// handle query request
		case IOCTL_QUERY: {
			// Read state on specified CPU.
			// ioctl_param contains the cpu to query as int. Pass it to
			// the query function; the function will return the recovered
			// state in info.
			uint64_t info = (int)ioctl_param;
			on_each_cpu(query, &info, true);

			// re-encode
			long retval = 0;
			if (info & MSR_DISABLE_LOAD_STORE_HW_PF) {
				retval |= PFSTATE_DISABLE_LOAD_STORE_HW_PF;
			}
			if (info & MSR_L1_L2_PREFETCH_CROSS_4KB) {
				retval |= PFSTATE_L1_L2_PREFETCH_CROSS_4KB;
			}
			if (info & MSR_LOAD_STORE_HW_PF_USE_VA) {
				retval |= PFSTATE_LOAD_STORE_HW_PF_USE_VA;
			}
			if (info & MSR_DISABLE_PF_READUNIQUE) {
				retval |= PFSTATE_DISABLE_PF_READUNIQUE;
			}
			if (info & MSR_DISABLE_L1I_PREFETCH) {
				retval |= PFSTATE_DISABLE_L1I_PREFETCH;
			}
			if (info & MSR_DISABLE_L2_TLB_PREFETCH) {
				retval |= PFSTATE_DISABLE_L2_TLB_PREFETCH;
			}

			return retval;
		}
	}
	return -1;
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

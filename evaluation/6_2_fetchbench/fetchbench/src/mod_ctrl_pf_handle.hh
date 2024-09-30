#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "../../mod-ctrlpf/ctrlpf_ioctl.h"

#if ! (defined(__x86_64__) || (defined(__aarch64__) && ! defined(__APPLE__)))
	#error Architecture not supported
#endif

/**
 * Wrapper around the file handle to the mod-pfctrl kernel module
 */
class ModCtrlPfHandle {
public:
	int fd;
	ModCtrlPfHandle();
	~ModCtrlPfHandle();
	// Delete copy constructor and copy-assignment. Implicitly also deletes
	// move constructor/assignment.
	ModCtrlPfHandle(ModCtrlPfHandle const&) = delete;
	ModCtrlPfHandle& operator=(ModCtrlPfHandle const&) = delete;
};
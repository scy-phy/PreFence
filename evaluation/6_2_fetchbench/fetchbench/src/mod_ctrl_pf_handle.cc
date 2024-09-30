#include "mod_ctrl_pf_handle.hh"

ModCtrlPfHandle::ModCtrlPfHandle() {
	fd = open(DEVICE_FILE_PATH, 0);
	if (fd < 0) {
		perror("Can't open device file " DEVICE_FILE_PATH);
		exit(-1);
	}
}

ModCtrlPfHandle::~ModCtrlPfHandle() {
	close(fd);
}
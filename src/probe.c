#include <stdio.h>
#include <sys/ioctl.h>
#include "src/midres_hw_io.h"

void main()
{
	int mode = 2;
	console_ioctl(IOCTL_GENCON_SET_MODE, &mode);
	unsigned char key;

	while (1) {
		//printf("KEY = %x T = %x %x %x\n", (*(unsigned char*)0x822d), (*(unsigned char*)(0x81cf)), (*(unsigned char*)(0x81cf + 1)), (*(unsigned char*)(0x81cf + 2)));
		printf("KEY = %d T = %x %x %x\n", (*(unsigned char*)0x822c), (*(unsigned char*)(0x81ce)), (*(unsigned char*)(0x81ce+1)), (*(unsigned char*)(0x81ce+2)));
	}

}
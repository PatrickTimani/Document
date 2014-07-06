/**
 *	\file fpgalib.h
 *	\brief FPGA access functions
 *	\author Marc Juettner, marc.juettner@juettner-itconsult.de
 *	\version 1.0
 *
 */
#if !defined _fpgalib_h_
#define _fpgalib_h_

#include "scrambler_ioctl.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

//DEFINES for use within the FPGA library...

#define DEFAULT_GENERATOR_POLYNOM	0x0000

#define FPGA_DEVICEFILE		"/dev/scrambler"

//FPGA Library status information
#define FPGA_STATUS_OFF		0
#define FPGA_STATUS_ON		1

//ERROR CODES for FPGA library functions
#define EFPGA_NOERROR		0
#define EFPGA_FILE_OPEN_ERROR	1
#define EFPGA_STATUS_OFF	2
#define EFPGA_WRITE_ERROR	3
#define EFPGA_READ_ERROR	4
#define EIOCTL_ERROR		5

//Function prototypes
int FPGA_Open(void);
int FPGA_Close(void);

int FPGA_SetGeneratorPolynom(int GP);
int FPGA_Scramble(int operand, int *result);

#endif //#define _fpgalib_h_

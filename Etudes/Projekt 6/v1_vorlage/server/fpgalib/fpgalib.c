/**
 *	\file fpgalib.c 
 *	\author Marc Juettner, marc.juettner@juettner-itconsult.de
 *	\version 1.0
 *	\brief An FPGA access library that implements access to a scrambler functionality 
 *		within an FPGA.
 *	\defgroup fpga FPGA
 *	\{
 *	\par Overview
 *	This library needs an FPGA design to be loaded into the FPGA chip before running any
 *	application that uses it. Furthermore, a corresponding device driver has to be loaded 
 *	correctly to be able to access the scrambler and a device file whose major id has to 
 *	correspond to the driver's major id has to exists under /dev. The scrambler is accessed 
 *	via read(), write() and ioctl() system calls on /dev/scrambler, see fpgalib.h for 
 *	further details.
 *
 *	\par Prerequisites
 *	The following files should be available on your system:
 *	\li scrambler.exp: An FPGA design containing a scrambler implementation
 *	\li load_fpga: A command line utility that loads an FPGA design (*.exo) given as first 
 *			argument into the FPGA chip
 *	\li scrambler.o: A device driver to access the design from userspace
 *
 *	\par Static variables
 *	\li iFPGAFileDesc will hold the device file descriptor returned by open() to easily 
 *	access the device once it is opened.
 *	\li iFPGAStatus will hold information on the library's operating status. Any function 
 *	that requires proper initialization of the FPGA device file(s) will first check this 
 *	variable and return with an error code (see fpgalib.h for details) if accessing the 
 *	device file would create a file not open error.
 *
 *	\warning Don't attempt to use the statically declared variables outside the library!
 *	\warning This library is NOT thread-safe. This implies that it is impossible to access 
 *	the FPGA from multiple applications simultaneously as the FPGA will remain
 *	opened between calls of FPGA_Open() and FPGA_Close(). As a workaround a userspace 
 *	application has to call FPGA_Open() before and FPGA_Close() after each access to the 
 *	FPGA. Be aware of the non-reentrancy of the library code! You won't be able to use it 
 *	in multithreaded applications without modification!
 *	\}
 */
#include "fpgalib.h"

/**
 *	\ingroup fpga
 *	\defgroup fpgalib FPGA library functions
 *
 * 	\{
 */

static int iFPGAFileDesc = 0;
static int iFPGAStatus = FPGA_STATUS_OFF;

/**
 *	\brief Open and initialize the FPGA library
 *	\return Zero if successfully opened FPGA, nonzero otherwise
 *	\see 	fpgalib.h
 *
 *	The function starts initialization of the FPGA library and should be called at the 
 *	beginning of any program that wants to access the FPGA design via device files. 
 */
int FPGA_Open(void) 
{
	int iReturn = 0;

	//get file descriptor for scrambler device file
	iFPGAFileDesc = open(FPGA_DEVICEFILE, O_RDWR);
	if( iFPGAFileDesc < 0 ) {
		//error opening device - break and exit with return code
		close(iFPGAFileDesc);
		iFPGAStatus = FPGA_STATUS_OFF;
		return -EFPGA_FILE_OPEN_ERROR;
	}

	//call ioctl function to set initial generator polynom
	iReturn = ioctl(iFPGAFileDesc, IOCTL_INIT_POLYGEN, DEFAULT_GENERATOR_POLYNOM);
	if( iReturn < 0 ) {
		//error initializing device - break and exit with return code
		close(iFPGAFileDesc);
		iFPGAStatus = FPGA_STATUS_OFF;
		return -EIOCTL_ERROR;
	}

	iFPGAStatus = FPGA_STATUS_ON;

	return EFPGA_NOERROR;
}

/**
 *	\brief Close the FPGA library
 *	\return Zero if successfully closed FPGA, nonzero otherwise
 *	\see 	fpgalib.h
 *
 *	The function will close the FPGA device file if it was previuosly opened and sets 
 *	iFPGAStatus to OFF. If the FPGA device is not already opened, the function will 
 *	return an error code.
 */
int FPGA_Close(void) {
	
	if (iFPGAStatus != FPGA_STATUS_ON) return -EFPGA_STATUS_OFF;
	close (iFPGAFileDesc);
	iFPGAFileDesc = 0;
	iFPGAStatus = FPGA_STATUS_OFF;
	return EFPGA_NOERROR;
}

/**
 *	\brief Set scrambler generator polynom
 *
 *	\param GP	An integer value representing the generator polynom
 *	\return 	Zero if successfully set the generator polynom, nonzero otherwise 
 *	\see 		fpgalib.h
 *
 *	The function will set the scrambler's generator polynom for further use 
 *	with FPGA_Scramble().
 */
int FPGA_SetGeneratorPolynom(int GP)
{
	int iReturn = 0;
	
	if (iFPGAStatus != FPGA_STATUS_ON) return -EFPGA_STATUS_OFF;

	iReturn = ioctl(iFPGAFileDesc, IOCTL_INIT_POLYGEN, GP);
	if( iReturn < 0 ) {
		//error initializing device - break and exit with return code
		close(iFPGAFileDesc);
		iFPGAStatus = FPGA_STATUS_OFF;
		return -EIOCTL_ERROR;
	}

	return EFPGA_NOERROR;
}

/**
 *	\brief Scramble data
 *
 *	\param operand	An integer value as scrambler input value
 *	\param result	An integer pointer pointing to a variable the scrambler result 
 *			is to be written to
 *	\return 	Zero if successfully executed, nonzero otherwise
 *	\see 		fpgalib.h
 *
 *	FPGA_Scramble() is mainly a wrapper function around the corresponding device file 
 *	write() and read() sequence that has to be called to scramble an operand.
 */
int FPGA_Scramble(int operand, int *result) {

	int iReturn = 0;

	if (iFPGAStatus != FPGA_STATUS_ON) return -EFPGA_STATUS_OFF;

	iReturn = write(iFPGAFileDesc, (char *)&operand, sizeof(operand));
	if( iReturn < 0 ) {
		//error writing to device - break and exit with return code
		FPGA_Close();
		return -EFPGA_WRITE_ERROR;
	}

	iReturn = read(iFPGAFileDesc, (char *)result, sizeof(int));

	if( iReturn < 0 ) {
		//error reading from device - break and exit with return code
		FPGA_Close();
		return -EFPGA_READ_ERROR;
	}

	return EFPGA_NOERROR;
}

/**
 *	\}
 */

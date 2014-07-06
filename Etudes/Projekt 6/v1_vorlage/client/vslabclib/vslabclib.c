/**
 *	\file vslabclib.c 
 *	\author Marc Juettner, marc.juettner@juettner-itconsult.de
 *	\author Manuel Gaiser, manuel.gaiser@hs-pforzheim.de
 *	\version 1.0
 *	\brief A library that implements access to a remote vslab server
 *	
 *	\defgroup vslabclib VSLab client library
 *	\{
 * \par Prerequisites
 * This library needs a running vslab server deamon on a properly configured hardware platform 
 * before running any application that uses it.
 *
 * \warning This library is not thread-safe!
 * It is impossible to access the vslab client library from multiple applications 
 * simultaneously as multiple parallel calls to vslcl_call_function() would cause 
 * received packets not to be assigned properly to the process or thread that expects it. 
 * There is much to do to make this library useable in multithreaded applications!
 * \}
 */
#include "vslabclib.h"

/**
 *	\ingroup vslabclib
 *	\defgroup vslcl_statvar Static variables
 *	\{
 */

/**
 *	Declaration of static variables that work across library functions - don't attempt to 
 *	use them outside the library!
 */

/**
 *	\brief The socket descriptor for remote vslab server access.
 *
 *	iVSLSocket will hold the socket descriptor returned by socket() to easily access
 *	the remote device once it is opened. Remember that the socket descriptor is not the only
 *	way to identify a remote system - here the socket descriptor is used only as a 
 *	descriptor refering to ONE socket that is used for any server the library communicates 
 *	with.
 *
 */
static int iVSLSocket = 0;

/**
 *	\brief The library status.
 *
 * 	iVSLCLStatus will hold information on the library's operating status. Any function 
 *	that requires proper initialization of the library will first check this variable and 
 *	return with an error code if accessing the library would create an error.
 *
 */
static int iVSLCLStatus = VSLCL_STATUS_OFF;

/**
 *	\brief Local socket information.
 *
 *	vsls_local holds information about the local system.
 */
static struct sockaddr_in	vsls_local;

/**
 *	\brief Remote socket information.
 *
 *	vsls_remote holds information about the remote system.
 */
static struct sockaddr_in	vsls_remote;

/**
 *	\brief Hold packets being sent by library functions.
 *
 */
static char sndpacket[PL_PACKETSIZE];

/**
 *	\brief Hold packets being received by library functions.
 *
 */
static char rcvpacket[PL_PACKETSIZE];

/**
 *	\brief Remote system IP address
 *
 *	Unicast_addr holds the unicast address of the remote system being used by the library. 
 *	As this library does not support multiple server nodes, a call to this function BEFORE 
 *	vslcl_Open() gives us the ability to override the default setting of VSLS_UNICAST_ADDRESS.
 */
static char unicast_addr[IP_ADDR_LEN] = VSLS_UNICAST_ADDRESS;

/**
 *	\brief Target node data structure
 *
 *	vsls_data is an instance of struct pl_data (see packetlib.h) that is mainly used 
 *	by vslcl_call_function() to communicate with the target node.
 */
static struct pl_data vsls_data;

/**
 *	\}
 */

/**
 *	\ingroup vslabclib
 *	\defgroup vslcl_func Library functions
 *	\{
 */

/**
 *	\brief Initialize vslab client library.
 *	\return Zero if successfully opened socket, error code otherwise
 *
 *	vslcl_Open() will start initialization of the vslab client library and should be called at 
 *	the beginning of any program that wants to access the remote node. vslcl_Open() will  
 *	return zero if successfully executed and an error code less than zero otherwise.
 */
int vslcl_Open(void)
{
	int iReturn = 0;
	
	//get socket descriptor
	iVSLSocket=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (iVSLSocket < 0)
	{
		iVSLCLStatus = VSLCL_STATUS_OFF;
		return -EVSLCL_SOCKET;
	}

   	// initialize client structure - that's me
   	vsls_local.sin_family = AF_INET;			// Ethernet
   	vsls_local.sin_addr.s_addr = htonl(INADDR_ANY);	// automatically insert own address
   	vsls_local.sin_port = htons(0);			// set vslab server port
   	memset(&(vsls_local.sin_zero), 0x00, 8);		// set remaining bytes to 0x0

	// initialize server structure - there's nothing to be done by OS, we have to set it
	// "at our own risk"...
	vsls_remote.sin_family = AF_INET;			// Ethernet
	vsls_remote.sin_addr.s_addr = inet_addr(unicast_addr);
	vsls_remote.sin_port = htons(VSLS_PORT);
	memset(&(vsls_remote.sin_zero), 0x00, 8);

	// bind socket
	iReturn = bind(iVSLSocket, (struct sockaddr *)&vsls_local, sizeof(struct sockaddr));
	if (iReturn < 0)
	{
		close(iVSLSocket);
		iVSLCLStatus = VSLCL_STATUS_OFF;
		return -EVSLCL_BIND;
	}

	// set library status
	iVSLCLStatus = VSLCL_STATUS_ON;

	return EVSLCL_NOERROR;
}


/**
 *	\brief Close the library
 *
 * 	\return Zero if successfully closed library, nonzero otherwise
 *
 *	vslcl_Close() will close the library if it was previuosly opened and sets 
 *	iVSLCLStatus to OFF. If the library is not already opened, vslcl_Close() 
 *	will return an error code.
 */
int vslcl_Close(void)
{
	// check library status
	if (iVSLCLStatus != VSLCL_STATUS_ON) return -EVSLCL_STATUS_OFF;

	// close socket
	close (iVSLSocket);

	// set library status
	iVSLCLStatus = VSLCL_STATUS_OFF;

	return EVSLCL_NOERROR;
}


/**
 *	\brief Execute function on remote node
 *
 *	\param fid 	An integer value specifying the requested function id
 *	\param param	An integer pointer pointing to an array of integer values representing
 *			the operands required for the function call. One has to assure that 
 *			there is enough space (i.e. PL_OPERAND_COUNT elements) for the array!
 *	\return		Zero if function executed successfully, error code otherwise
 *
 */
int vslcl_call_function(int fid, int *param)
{
	unsigned int i = 0, iSndLen = 0, iRcvLen = 0;

	// create request packet...
	pl_create_request(&vsls_data);

	// set function id
	PLM_FUNCTION_ID(vsls_data) = fid;

	// set operands
	for (i=0; i < PL_OPERAND_COUNT; i++) PLM_OPERAND(vsls_data, i) = param[i];

	// serialize packet
	pl_make_packet(&vsls_data, sndpacket, PL_PACKETSIZE);

	// send packet
	iSndLen = sendto(iVSLSocket, &sndpacket, PL_PACKETSIZE, 0, (struct sockaddr*)&vsls_remote, sizeof(struct sockaddr));

	// receive packet and check for timeout error
	i = sizeof(struct sockaddr);
	tol_start_timeout(5);
	iRcvLen = recvfrom(iVSLSocket, &rcvpacket, PL_PACKETSIZE, 0, (struct sockaddr*)&vsls_remote, &i);
	tol_stop_timeout();
	if (tol_is_timed_out()) {
		return -EVSLCL_NET_TIMEOUT;
	}

	// extract packet received
	pl_extr_packet(rcvpacket, &vsls_data, iRcvLen);

	// copy returned values...
	for (i=0; i < PL_OPERAND_COUNT; i++) param[i] = PLM_OPERAND(vsls_data, i);

	// create return value according to the returned packet...	
	if (PLM_PACKET_TYPE(vsls_data) == PL_PTYPE_ERR) return -PLM_OPERAND(vsls_data, 0);
	if (PLM_PACKET_TYPE(vsls_data) == PL_PTYPE_RSP) return EVSLCL_NOERROR;
	return -EVSLCL_UNKNOWN_ERROR;
}


/**
 *	\brief	Call multiply function
 *
 *	\param op1	Operand 1, an integer value
 *	\param op2	Operand 2, an integer value
 *	\param result	An integer pointer pointing to a variable the result 
 *			is to be written to
 *	\return 	Zero if successfully executed, error code otherwise
 *
 */
int vslcl_Multiply(int op1, int op2, int *result)
{
	int iReturn = 0, params[PL_OPERAND_COUNT];
	unsigned int i = 0;

	// check library status
	if (iVSLCLStatus != VSLCL_STATUS_ON) return -EVSLCL_STATUS_OFF;

	// set operands
	params[0] = op1;
	params[1] = op2;
	for (i = 2; i < PL_OPERAND_COUNT; i++) params[i] = 0;

	// call vslab server function
	iReturn = vslcl_call_function(PL_FID_MUL, params);

	// set return values
	if (iReturn<0) return iReturn;
	*result = params[0];
	return iReturn;

}


/**
 *	\brief	Call divide function
 *
 *	\param op1	Operand 1, an integer value
 *	\param op2	Operand 2, an integer value
 *	\param result	An integer pointer pointing to a variable the result 
 *			is to be written to
 *	\return 	Zero if successfully executed, error code otherwise
 *
 */
int vslcl_Divide(int op1, int op2, int *result)
{
	int iReturn = 0, params[PL_OPERAND_COUNT];

	// check library status
	if (iVSLCLStatus != VSLCL_STATUS_ON) return -EVSLCL_STATUS_OFF;

	// set operands
	params[0] = op1;
	params[1] = op2;
	for (iReturn = 2; iReturn < PL_OPERAND_COUNT; iReturn++) params[iReturn] = 0;

	// call vslab server function
	iReturn = vslcl_call_function(PL_FID_DIV, params);

	// set return values
	if (iReturn<0) return iReturn;
	*result = params[0];
	return iReturn;

}


/**
 *	\brief Set the remote unicast address
 *
 *	\param address	A string representing the unicast address to be used by
 *			vslcl_Open().
 *	\return 	Zero if successfully executed, nonzero otherwise
 *
 *	vslcl_SetUnicastAddress() sets the unicast address used by the library. It has to be 
 *	called BEFOFE vslcl_Open()! The function returns zero if successfully executed and an 
 *	error code otherwise.
 */
int vslcl_SetUnicastAddress(char *address) {

	if (address == NULL) return -EVSLCL_NULLPTR;
	if (strlen(address) > IP_ADDR_LEN) return -EVSLCL_WRONGADDRLEN;
	strncpy(unicast_addr, address, IP_ADDR_LEN);
	return EVSLCL_NOERROR;
}


/**
 *	\}
 */

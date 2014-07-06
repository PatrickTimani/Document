/**
 *	\file vslabclib.h
 *	\brief Definitions for vslab client lib
 *	\author Marc Juettner, marc.juettner@juettner-itconsult.de
 *	\author Manuel Gaiser, manuel.gaiser@hs-pforzheim.de
 *	\version 1.0
 *
 */
#if !defined _vslabclib_h_
#define _vslabclib_h_

#include "../../packetlib/packetlib.h"
#include "../../timeoutlib/timeoutlib.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>


// defines for use within the vslab client library...

/** \brief Default server port. 
 *
 * The port of the vslab server.
 */
#define VSLS_PORT		11111

/** \brief Default server IP. 
 *
 * The default IP of the vslab server.
 */
#define VSLS_UNICAST_ADDRESS	"141.47.69.14"

/** \brief IP address length. 
 *
 * Number of bytes to be reserved for server IP address.
 */
#define IP_ADDR_LEN		16


// vslab client library states
/** \brief Library status. 
 *
 * vslab client library status is set to 1 when the socket interface is opened and bound.
 */
#define VSLCL_STATUS_OFF	0
#define VSLCL_STATUS_ON		1


// error codes of vslabclib functions
/** \brief No error. */
#define EVSLCL_NOERROR	0

/** \brief Bind error. 
 *
 * An error occurred during socket binding.
 */
#define EVSLCL_BIND		101

/** \brief Socket error. 
 *
 * An error occured while creating a socket.
 */
#define EVSLCL_SOCKET		102

/** \brief Library not opened.  
 *
 * A library function was called without previous call to vslcl_Open().
 */
#define EVSLCL_STATUS_OFF	103

/** \brief Unknown error.  
 *
 * An unknown error happened.
 */
#define EVSLCL_UNKNOWN_ERROR	104

/** \brief Network timeout error.  
 *
 * Network timed out.
 */
#define EVSLCL_NET_TIMEOUT	105

/** \brief Null pointer argument passed to function call.  
 *
 * A null pointer was passed to a function.
 */
#define EVSLCL_NULLPTR	106
		

/** \brief Wrong address length.  
 *
 * The given IP address doesn't have the correct length.
 */
#define EVSLCL_WRONGADDRLEN	107

/** \brief Library already opened.  
 *
 * There was a second call to vslcl_Open().
 */
#define EVSLCL_STATUS_ON	108


// vslab client library function prototypes
int vslcl_Open(void);
int vslcl_Close(void);
int vslcl_Multiply(int op1, int op2, int *result);
int vslcl_Divide(int op1, int op2, int *result);
int vslcl_SetUnicastAddress(char *address);

#endif //#define _vslabclib_h_

/**
 *	\file vslabd.h
 *	\brief VSLab daemon: General defines
 *	\author Marc Juettner, marc.juettner@juettner-itconsult.de
 *	\author Manuel Gaiser, manuel.gaiser@hs-pforzheim.de
 *	\version 1.0
 *
 */
#if !defined _vslabd_h_
#define _vslabd_h_


// defines for use within the VSLab server library...
/** \brief VSLab server version. 
 *
 * The version of the VSLab server.
 */
#define VSLD_VERSION			"lab_1_template"

/** \brief Default server port. 
 *
 * The port of the VSLab server.
 */
#define VSLD_PORT			11111

/** \brief Timeout duration. 
 *
 * Number of seconds to wait for a receive packet.
 */
#define VSLD_TIMEOUT_SECS		10


// error codes
/** \brief Socket error. 
 *
 * An error occured while creating a socket.
*/
#define ESOCKET				1

/** \brief Bind error. 
 *
* An error occurred during socket binding.
 */
#define EBIND				2


#endif //#define _vslabd_h_

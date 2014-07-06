/**
 *	\file packetlib.h
 *	\brief Definitions for packet handling
 *	\author Marc Juettner, marc.juettner@juettner-itconsult.de
 *	\author Manuel Gaiser, manuel.gaiser@hs-pforzheim.de
 *	\version 1.1
 *
 */
#if !defined _packetlib_h_
#define _packetlib_h_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

// packet types
/** \brief Request operation */
#define PL_PTYPE_REQ	1
/** \brief Respond to operation request */
#define PL_PTYPE_RSP	2
/** \brief Error packet */
#define PL_PTYPE_ERR	3

// packet modes
/** \brief Client mode */
#define PL_MODE_CLN	1
/** \brief Server mode */
#define PL_MODE_SRV	2

// function IDs
/** \brief Multiplication */
#define PL_FID_MUL		1	
/** \brief Division */
#define PL_FID_DIV		2

// error codes in server packets
/** \brief General error. */
#define PL_ERR_GENERALERROR	1
/** \brief Invalid packet type. */
#define PL_ERR_INVALIDTYPE	2
/** \brief Invalid packet mode. */
#define PL_ERR_INVALIDMODE	3
/** \brief Function execution error. */
#define PL_ERR_FUNCEXECERROR	4
/** \brief No such function. */
#define PL_ERR_NOSUCHFUNCTION	5

// error codes of packetlib functions
/** \brief No error. */
#define E_PL_NOERROR			0
/** \brief NULL pointer. 
 * NULL pointer in function parameter.
 */
#define E_PL_NULLPTR			1
/** \brief Insufficient buffer size. 
 * Buffer size not sufficient.
 */
#define E_PL_INSUFFICIENTBUFFER		2


// indices for packet content byte adressing
#define PL_PIDX_TYPE	0
#define PL_PIDX_MODE	(1*4)
#define PL_PIDX_FID	(2*4)
#define PL_PIDX_OP(x)	((3+x)*4)


// definitions for operand count and packet size
#define PL_OPERAND_COUNT	2
#define PL_PACKETSIZE		(sizeof(struct pl_data))


// Some macros that shall make the daemon code more readable
// Macro names start with PLM_
/** 
 *      \brief Get the packet type
 *	\param x	An instance of struct pl_data
 *	\return		packet type
 */
#define PLM_PACKET_TYPE(x)	x.type

/** 
 *      \brief Get the packet mode
 *	\param x	An instance of struct pl_data
 *	\return		packet mode
 */
#define PLM_PACKET_MODE(x)	x.mode

/** 
 *      \brief Get the packet function ID
 *	\param x	An instance of struct pl_data
 *	\return		packet function id
 */
#define PLM_FUNCTION_ID(x)	x.function_id

/** 
 *	\brief Return a packet operand
 *	\param x	An instance of struct pl_data
 *	\param y	The requested operand 
 *	\return		The requested operand's value.
 */
#define PLM_OPERAND(x, y)	x.data[y]

/**
 *	\brief packet data structure
 *	This structure represents the core data structure of the protocol.
 */
struct pl_data {
	unsigned int type;			/**< \brief The packet type. */
	unsigned int mode;			/**< \brief The packet mode. */
	unsigned int function_id;		/**< \brief The function ID. */
	unsigned int data[PL_OPERAND_COUNT];	/**< \brief The packet's operands. */
};

// Function prototypes
int pl_make_packet(struct pl_data *, char *, unsigned int);
int pl_extr_packet(char*, struct pl_data *, unsigned int);
int pl_create_response(struct pl_data *);
int pl_create_request(struct pl_data *);
int pl_create_error(struct pl_data *, int);

#endif //#define _packetlib_h_

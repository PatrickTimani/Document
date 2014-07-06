/**
 *	\file packetlib.c
 *	\brief Function definitions for packet handling
 *	\author Marc Juettner, marc.juettner@juettner-itconsult.de
 *	\author Manuel Gaiser, manuel.gaiser@hs-pforzheim.de
 *	\version 1.1
 */
#include "packetlib.h"

/**
 *	\defgroup packetlib Packet handling
 *	\{
 */

/**
 *	\brief Serialize a packet structure
 *	\param data	A pointer to a struct pl_data containing the data to be
 *			serialized.
 *	\param packet	A pointer to a target character buffer
 *	\param len	The size of the target buffer given by \a packet
 *	\return		E_PL_NOERROR if successful, an error code otherwise
 */
int pl_make_packet(struct pl_data *data, char* packet, unsigned int len)
{
	int i = 0;

	if ((data == NULL) || (packet == NULL)) 
	{
		printf("Error creating packet!\n");
		return -E_PL_NULLPTR;
	}
	if (len < PL_PACKETSIZE) return -E_PL_INSUFFICIENTBUFFER;

	*(int*)(&packet[PL_PIDX_TYPE]) = htonl(data->type);
	*(int*)(&packet[PL_PIDX_MODE]) = htonl(data->mode);
	*(int*)(&packet[PL_PIDX_FID]) = htonl(data->function_id);
	for (i = 0; i<PL_OPERAND_COUNT; i++) *(int*)(&packet[PL_PIDX_OP(i)]) = htonl(data->data[i]);

	return E_PL_NOERROR;
}

/**
 *	\brief Unserialize a packet structure
 *	\param packet	A pointer to a source character buffer
 *	\param data	A pointer to a struct pl_data for the data to be
 *			unserialized.
 *	\param len	The size of the source buffer given by \a packet
 *	\return		E_PL_NOERROR if successful, an error code otherwise
 */
int pl_extr_packet(char* packet, struct pl_data *data, unsigned int len)
{
	int i = 0;

	if ((data == NULL) || (packet == NULL)) 
	{
		printf("Error extracting packet!\n");
		return -E_PL_NULLPTR;
	}
	if (len < PL_PACKETSIZE) return -E_PL_INSUFFICIENTBUFFER;

	data->type = ntohl(*(int*)(&packet[PL_PIDX_TYPE]));
	data->mode = ntohl(*(int*)(&packet[PL_PIDX_MODE]));
	data->function_id = ntohl(*(int*)(&packet[PL_PIDX_FID]));
	for (i = 0; i<PL_OPERAND_COUNT; i++) data->data[i] = ntohl(*(int*)(&packet[PL_PIDX_OP(i)]));
	
	return E_PL_NOERROR;
}

/**
 *	\brief Create a response packet
 *	\param data	A pointer to a struct pl_data for the data to be
 *			filled in.
 *	\return		E_PL_NOERROR if successful, an error code otherwise
 *
 *	The function sets the fields in the given structure according to the requirements
 *	for a response packet.
 */
int pl_create_response(struct pl_data *data)
{
	if (data == NULL) return -E_PL_NULLPTR;
	
	data->type = PL_PTYPE_RSP;
	data->mode = PL_MODE_SRV;

	return E_PL_NOERROR;
}

/**
 *	\brief Create a request packet
 *	\param data	A pointer to a struct pl_data for the data to be
 *			filled in.
 *	\return		E_PL_NOERROR if successful, an error code otherwise
 *
 *	The function sets the fields in the given structure according to the requirements
 *	for a request packet.
 */
int pl_create_request(struct pl_data *data)
{
	if (data == NULL) return -E_PL_NULLPTR;
	
	data->type = PL_PTYPE_REQ;
	data->mode = PL_MODE_CLN;

	return E_PL_NOERROR;
}

/**
 *	\brief Create an error packet
 *	\param data		A pointer to a struct pl_data for the data to be filled in.
 *	\param errorcode	The error code to be used as operand 0
 *	\return			E_PL_NOERROR if successful, an error code otherwise
 *
 *	The function sets the fields in the given structure according to the requirements
 *	for an error packet. The errorcode is copied into operand 0.
 */
int pl_create_error(struct pl_data *data, int errorcode)
{
	if (data == NULL) return -E_PL_NULLPTR;
	
	data->type = PL_PTYPE_ERR;
	data->mode = PL_MODE_SRV;
	data->data[0] = errorcode;

	return E_PL_NOERROR;
}


/**
 *	\}
 */

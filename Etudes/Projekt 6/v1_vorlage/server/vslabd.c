/**
 *	\file vslabd.c
 *	\brief The VSLab daemon
 *	\author Marc Juettner, marc.juettner@juettner-itconsult.de
 *	\author Manuel Gaiser, manuel.gaiser@hs-pforzheim.de
 *	\version 1.0
 *
 *	\defgroup vslabdaemon VSLab daemon implementation
 *	\{
 */
#include "includes.h"

int main(void)
{
	int iReturn = 0, iResult = 0;
	int iVSLSocket = 0;
	int iRcvLen = 0, iSndLen = 0;
	unsigned int i;
	
	struct pl_data vsld_data;
	
	struct sockaddr_in vsld_remote, vsld_local;
	char sndpacket[PL_PACKETSIZE];
	char rcvpacket[PL_PACKETSIZE];

	// introducing myself...
	printf("VSLab server daemon, version %s, build %s %s\n", VSLD_VERSION, __DATE__, __TIME__);

	// initializing 7seg display driver
	sevenseg_open();

	// get a socket descriptor from OS
	iVSLSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (iVSLSocket < 0)
	{
		perror("vslabd: Error creating socket.\n");
		return -ESOCKET;
	}

	// initialize server description structure - that's me
   	vsld_local.sin_family = AF_INET;			// Ethernet
   	vsld_local.sin_addr.s_addr = htonl(INADDR_ANY);		// automatically insert own address
   	vsld_local.sin_port = htons(VSLD_PORT);			// set vslab server port
   	memset(&(vsld_local.sin_zero), 0x00, 8);		// set remaining bytes to 0x0

	// initialize client description structure - in fact we don't need to do this as this struct
	// will be populated by recvfrom() calls.
	vsld_remote.sin_family = AF_INET;			// Ethernet
	vsld_remote.sin_addr.s_addr = htonl(INADDR_ANY);
	vsld_remote.sin_port = htons(VSLD_PORT);
	memset(&(vsld_remote.sin_zero), 0x00, 8);

	// bind socket
	iReturn = bind(iVSLSocket, (struct sockaddr *)&vsld_local, sizeof(struct sockaddr));
	if (iReturn < 0)
	{
		perror("vslabd: Error binding to socket.\n");
		close(iVSLSocket);
		return -EBIND;
	}

	// report status to 7seg display	
	sevenseg_setch('0');

	// main loop
	for (;;) {
		// wait for incoming requests
		i = sizeof(struct sockaddr);
		tol_start_timeout(VSLD_TIMEOUT_SECS);
		iRcvLen = recvfrom(iVSLSocket, &rcvpacket, PL_PACKETSIZE, 0, (struct sockaddr*)&vsld_remote, &i);
		tol_stop_timeout();
		if (tol_is_timed_out()) {
			tol_reset_timeout();
			printf("vslabd: Got a timeout. Restarting.\n");
			continue;
		}

		// extract incoming packet		
		iReturn = pl_extr_packet(rcvpacket, &vsld_data, iRcvLen);

		// check for error during packet extraction
		if(iReturn < 0) {
			pl_create_error(&vsld_data, PL_ERR_GENERALERROR);
		}
		// check packet type
		else if (PLM_PACKET_TYPE(vsld_data) != PL_PTYPE_REQ) {
			pl_create_error(&vsld_data, PL_ERR_INVALIDTYPE);
		}
		// check packet mode. We're a server, so we won't accept server packets!
 		else if (PLM_PACKET_MODE(vsld_data) != PL_MODE_CLN) {
			pl_create_error(&vsld_data, PL_ERR_INVALIDMODE);
		}
		// this is our core job - switch to the requested function id...
		else switch(PLM_FUNCTION_ID(vsld_data)) {
			case PL_FID_MUL:
				// multiply operands 0 and 1 of the received packet
				printf("vslabd: Calculating %d * %d...\n", PLM_OPERAND(vsld_data,0), PLM_OPERAND(vsld_data,1));
				iResult = PLM_OPERAND(vsld_data,0) * PLM_OPERAND(vsld_data,1);
				pl_create_response(&vsld_data);
				PLM_OPERAND(vsld_data, 0) = iResult;
				// Report status to 7seg display	
				sevenseg_setch('1');
				break;
			case PL_FID_DIV:
				// divide operand 0 by operand 1 of the received packet
				printf("vslabd: Calculating %d / %d...\n", PLM_OPERAND(vsld_data,0), PLM_OPERAND(vsld_data,1));
				// check if divisor is 0
				if (PLM_OPERAND(vsld_data,1) == 0) {
					pl_create_error(&vsld_data, PL_ERR_FUNCEXECERROR);
					// report status to 7seg display	
					sevenseg_setch('E');
				}
				else {
					iResult = PLM_OPERAND(vsld_data,0) / PLM_OPERAND(vsld_data,1);
					pl_create_response(&vsld_data);
					PLM_OPERAND(vsld_data, 0) = iResult;
					// report status to 7seg display	
					sevenseg_setch('2');
				}
				break;
			default:
				// function is not implemented - create an error packet
				pl_create_error(&vsld_data, PL_ERR_NOSUCHFUNCTION);
				// report status to 7seg display	
				sevenseg_setch('F');
				break;
		}

		// convert and send packet
		pl_make_packet(&vsld_data, sndpacket, PL_PACKETSIZE);
		iSndLen = sendto(iVSLSocket, &sndpacket, PL_PACKETSIZE, 0, (struct sockaddr*)&vsld_remote, sizeof(struct sockaddr));
	}
	sevenseg_close();
	return 0;
}

/**
 *	\}
 */

/**
 *	\file vslabc.c
 *	\brief The VSLab client
 *	\author Marc Juettner, marc.juettner@juettner-itconsult.de
 *	\author Manuel Gaiser, manuel.gaiser@hs-pforzheim.de
 *	\version 1.0
 *
 *	\defgroup vslabclient VSLab client implementation
 *	\{
 */
#include "includes.h"

int main(int argc, char **argv)
{
	int iReturn = 0;
	int iOp1, iOp2;
	unsigned char cFunc;
	int iResult;
	
	// introduce yourself...
	printf("VSLab client, version %s, build %s %s\n", VSLC_VERSION, __DATE__, __TIME__);

	// check command line parameters
	if (argc < 5) {
		printf("Missing arguments!\n");
		printf("Usage: vslabc op1 op2 func ip\n");
		printf("Operands op1 and op2 must be integers.\n");
		printf("func = m -> Multiplication\n");
		printf("func = d -> Division\n");
		printf("ip = IP address of VSLab server\n");
		return -1;
	}

	// convert command line parameters to integers
	iOp1 = atoi(argv[1]);
	iOp2 = atoi(argv[2]);

	cFunc = argv[3][0];

	// set target IP
	vslcl_SetUnicastAddress(argv[4]);
	// initialize vslab client library
	vslcl_Open();

	// call function
	switch (cFunc) {
		case 'm': iReturn = vslcl_Multiply(iOp1, iOp2, &iResult); break; 
		case 'd': iReturn = vslcl_Divide(iOp1, iOp2, &iResult); break;
		default: printf("Function not defined!\n"); vslcl_Close(); return -1;
	}

	// evaluate return value
	if( iReturn < 0 ) 
	{
		printf("VSLab client: Got an error: %d\n", iReturn);
	}
	else {
		printf("VSLab client: Result: %d\n", iResult);
	}

	// close library
	vslcl_Close();

	return 0;
}

/**
 *	\}
 */

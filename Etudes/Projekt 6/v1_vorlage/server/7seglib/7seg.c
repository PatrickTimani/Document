/**
 *	\file 7seg.c Sevensegment display access
 *	\brief Functions to access the sevensegment display
 *	\author Marc Juettner, marc.juettner@juettner-itconsult.de
 *	\version 1.0
 *
 *	\warning These functions are NOT thread-safe!
 */
#include "7seg.h"

/**
 *	\ingroup netfpgadaemon
 *	\defgroup 7seg Sevensegment display access
 *
 * 	\{
 */
static int iFileDesc = 0;

/** 
 *	\brief Write character to sevensegment display
 *	\param ch	Character to write (0-9, A-F)
 *	\return		Zero if successful, negative value otherwise.
 *
 *	This function prints the given character to the sevensegment display if the
 *	character is displayable. Characters from binary 0 to binary 15 are converted
 *	to output of character '0' to 'f', e.g. writing 10 prints 'a' and so on.
 */
int sevenseg_setch(char ch) {
	
	if ( write(iFileDesc,&ch,1) < 0 )
	{
		printf("Fehler beim Schreiben auf die Ausgabedatei.\n");
		close(iFileDesc);
		return -3;
	}
	return 0;	
}

/** 
 *	\brief Open sevensegment display
 *	\return	Zero if successful, negative value otherwise
 *
 *	Opens the sevensegment display and stores the resulting file descriptor
 *	globally (implying that these functions are NOT thread-safe!).
 *	\note	The device file /dev/7segment has to exist and the corresponding
 *		driver has to be loaded.
 */
int sevenseg_open(void) {
	
	iFileDesc=open("/dev/7segment",O_WRONLY);
	if ( iFileDesc < 0 )
	{	//Fehler beim �ffnen der Datei
		printf("Fehler beim oeffnen von /dev/7segment.\n");
		return -2;
	}
// 	printf("%s erfolgreich ge�ffnet.\n",DEVICE_NAME);
	return 0;
}

/** 
 *	\brief Open sevensegment display
 *	\return	Zero
 *
 *	Closes the sevensegment display.
 */
int sevenseg_close(void) {
	close(iFileDesc);
	return 0;
}
/**
 *	\}
 */

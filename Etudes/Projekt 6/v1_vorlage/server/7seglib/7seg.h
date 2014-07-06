/**
 *	\file 7seg.h Sevensegment display access (header)
 *	\brief Functions to access the sevensegment display
 *	
 *	\author Marc Juettner, marc.juettner@juettner-itconsult.de
 *	\version 1.0
 *
 */
#if !defined _7seg_h_
#define _7seg_h_

#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int sevenseg_setch(char ch);
int sevenseg_open(void);
int sevenseg_close(void);

#endif //#define _7seg_h_

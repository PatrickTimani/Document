/**
 *	\file scrambler_ioctl.h
 *	\brief Scrambler ioctl function IDs.
 *	\author Marc Juettner, marc.juettner@juettner-itconsult.de
 *	\version 1.0
 *
 */
#ifndef __SCRAMBLER_IOCTL_H
#define __SCRAMBLER_IOCTL_H

#include <linux/ioctl.h>

#define MAJORNUM 250
#define IOCTL_INIT_POLYGEN _IOR(MAJORNUM, 0, int*)

#endif

/*
    FreeRTOS V6.0.0 - Copyright (C) 2009 Real Time Engineers Ltd.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it    under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation and modified by the FreeRTOS exception.
    **NOTE** The exception to the GPL is included to allow you to distribute a
    combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    Alternative commercial license and support terms are also available upon
    request.  See the licensing section of http://www.FreeRTOS.org for full
    license details.

    FreeRTOS is distributed in the hope that it will be useful,    but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details.

    You should have received a copy of the GNU General Public License along
    with FreeRTOS; if not, write to the Free Software Foundation, Inc., 59
    Temple Place, Suite 330, Boston, MA  02111-1307  USA.


    ***************************************************************************
    *                                                                         *
    * The FreeRTOS eBook and reference manual are available to purchase for a *
    * small fee. Help yourself get started quickly while also helping the     *
    * FreeRTOS project! See http://www.FreeRTOS.org/Documentation for details *
    *                                                                         *
    ***************************************************************************

    1 tab == 4 spaces!

    Please ensure to read the configuration and relevant port sections of the
    online documentation.

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "portable.h"

/* Demo application include files. */
#include "partest.h"

/*-----------------------------------------------------------
 * Simple parallel port IO routines.
 *
 * This is for the demo application which uses port 2 for LED outputs.
 *-----------------------------------------------------------*/

/* Value for the LED to be off. */
#define partstLED_OUTPUTS		( ( unsigned char ) 0xff )

/* P2.0 is not used as an output so there are only 7 LEDs on port 2. */
#define partstMAX_LEDs			( 7 )
#define partstALL_OUTPUTS_OFF	( ( unsigned char ) 0 )

/* Maps the LED outputs used by the standard demo application files to
convenient outputs for the EDK2329.  Mainly this insures that the LED
used by the Check task is one of the on board LEDs so the demo can be
executed on an EDK without any modification. */
static inline unsigned char prvMapLED( unsigned portBASE_TYPE uxLED );

/*-----------------------------------------------------------*/

void vParTestInitialise( void )
{
	/* LED's are connected to port 2.  P2.1 and P2.2 are built onto the EDK.
	P2.3 to P2.7 are soldered onto the expansion port. */
	P2DDR = partstLED_OUTPUTS;
	P2DR = partstALL_OUTPUTS_OFF;
}
/*-----------------------------------------------------------*/

/*
 * Described at the top of the file.
 */
static inline unsigned char prvMapLED( unsigned portBASE_TYPE uxLED )
{
	switch( uxLED )
	{
		case 0	:	return ( unsigned char ) 2;
		case 1	:	return ( unsigned char ) 3;
		case 2	:	return ( unsigned char ) 4;
		case 3 	:	return ( unsigned char ) 5;
		case 4	:	return ( unsigned char ) 6;
		case 5	:	return ( unsigned char ) 0;
		case 6	:	return ( unsigned char ) 1;
		default	:	return 0;
	}
}
/*-----------------------------------------------------------*/

/*
 * Turn an LED on or off.
 */
void vParTestSetLED( unsigned portBASE_TYPE uxLED, signed portBASE_TYPE xValue )
{
unsigned char ucLED;

	if( uxLED < partstMAX_LEDs )
	{
		ucLED = prvMapLED( uxLED );

		/* Set a bit in the required LED position.  LED 0 is bit 1. */
		ucLED = ( unsigned char ) 1 << ( ucLED + 1 );

		if( xValue )
		{
			portENTER_CRITICAL();
				P2DR |= ucLED;
			portEXIT_CRITICAL();
		}
		else
		{
			portENTER_CRITICAL();
				P2DR &= ~ucLED;
			portEXIT_CRITICAL();
		}		
	}
}
/*-----------------------------------------------------------*/

void vParTestToggleLED( unsigned portBASE_TYPE uxLED )
{
unsigned char ucLED;

	if( uxLED < partstMAX_LEDs )
	{
		ucLED = prvMapLED( uxLED );

		/* Set a bit in the required LED position.  LED 0 is bit 1. */
		ucLED = ( unsigned char ) 1 << ( ucLED + 1 );

		portENTER_CRITICAL();
		{
			if( P2DR & ucLED )
			{
				P2DR &= ~ucLED;
			}
			else
			{
				P2DR |= ucLED;
			}
		}
		portEXIT_CRITICAL();
	}	
}




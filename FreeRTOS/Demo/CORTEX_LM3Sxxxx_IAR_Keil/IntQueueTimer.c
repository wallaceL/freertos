/*
    FreeRTOS V7.5.0 - Copyright (C) 2013 Real Time Engineers Ltd.

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that has become a de facto standard.             *
     *                                                                       *
     *    Help yourself get started quickly and support the FreeRTOS         *
     *    project by purchasing a FreeRTOS tutorial book, reference          *
     *    manual, or both from: http://www.FreeRTOS.org/Documentation        *
     *                                                                       *
     *    Thank you!                                                         *
     *                                                                       *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

    >>! NOTE: The modification to the GPL is included to allow you to distribute
    >>! a combined work that includes FreeRTOS without being obliged to provide
    >>! the source code for proprietary components outside of the FreeRTOS
    >>! kernel.

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available from the following
    link: http://www.freertos.org/a00114.html

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?"                                     *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/* Scheduler includes. */
#include "FreeRTOS.h"

/* Demo includes. */
#include "IntQueueTimer.h"
#include "IntQueue.h"

/* Library includes. */
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "interrupt.h"
#include "sysctl.h"
#include "lmi_timer.h"

#define tmrTIMER_2_FREQUENCY	( 2000UL )
#define tmrTIMER_3_FREQUENCY	( 2001UL )

void vInitialiseTimerForIntQueueTest( void )
{
unsigned long ulFrequency;

	/* Timer 2 and 3 are utilised for this test. */
	SysCtlPeripheralEnable( SYSCTL_PERIPH_TIMER2 );
    SysCtlPeripheralEnable( SYSCTL_PERIPH_TIMER3 );
    TimerConfigure( TIMER2_BASE, TIMER_CFG_32_BIT_PER );
    TimerConfigure( TIMER3_BASE, TIMER_CFG_32_BIT_PER );
	
	/* Set the timer interrupts to be above the kernel.  The interrupts are
	 assigned different priorities so they nest with each other. */
	IntPrioritySet( INT_TIMER2A, configMAX_SYSCALL_INTERRUPT_PRIORITY + ( 1 << 5 ) ); /* Shift left 5 as only the top 3 bits are implemented. */
	IntPrioritySet( INT_TIMER3A, configMAX_SYSCALL_INTERRUPT_PRIORITY );

	/* Ensure interrupts do not start until the scheduler is running. */
	portDISABLE_INTERRUPTS();
	
	/* The rate at which the timers will interrupt. */
	ulFrequency = configCPU_CLOCK_HZ / tmrTIMER_2_FREQUENCY;	
    TimerLoadSet( TIMER2_BASE, TIMER_A, ulFrequency );
    IntEnable( INT_TIMER2A );
    TimerIntEnable( TIMER2_BASE, TIMER_TIMA_TIMEOUT );

	/* The rate at which the timers will interrupt. */
	ulFrequency = configCPU_CLOCK_HZ / tmrTIMER_3_FREQUENCY;	
    TimerLoadSet( TIMER3_BASE, TIMER_A, ulFrequency );
    IntEnable( INT_TIMER3A );
    TimerIntEnable( TIMER3_BASE, TIMER_TIMA_TIMEOUT );

    /* Enable both timers. */	
    TimerEnable( TIMER2_BASE, TIMER_A );
    TimerEnable( TIMER3_BASE, TIMER_A );
}
/*-----------------------------------------------------------*/

void vT2InterruptHandler( void )
{
    TimerIntClear( TIMER2_BASE, TIMER_TIMA_TIMEOUT );	
	portEND_SWITCHING_ISR( xFirstTimerHandler() );
}
/*-----------------------------------------------------------*/

void vT3InterruptHandler( void )
{
	TimerIntClear( TIMER3_BASE, TIMER_TIMA_TIMEOUT );
	portEND_SWITCHING_ISR( xSecondTimerHandler() );
}



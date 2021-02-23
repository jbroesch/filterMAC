/**********************************************************************
*
* © James D. Broesch, 2011, 2013
* 
* This code demonstrates the use of mixed programming techniques.
* A vector MAC routine, written in assembly, is called from the C code.
*
* FileName:        main.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       dsPIC33FJ256GP506
* Compiler:        MPLAB® C30 v3.00 or higher
*
************************************************************************/
#include <math.h>

#include "p33FJ256GP506.h"
#include "WM8510CODEC.h"
#include "timer.h"
#include "dscInit.h"

// Declare the vector functions as external.
extern int vectorMAC(int *x, int *y, unsigned int loop_count,unsigned int scale);
extern int vectorShift(int *vector, int vectorLength);

extern unsigned int dataReady;
 
#define SIGNAL_SIZE 61

signed int signalIn;
signed int signalOut;

// The signal vector is placed in the x memory space.
//The coefficents are placed in the y memory space.
//This allows single cycle MAC operations.
signed int signal[SIGNAL_SIZE+1] __attribute__((space(xmemory), aligned));
signed int coefficents[SIGNAL_SIZE+1]__attribute__ ((space(ymemory), aligned)) 
 = {
        0xEDB6,
        0xEEE7,
        0xF258,
        0xF799,
        0xFE0C,
        0x04E6,
        0x0B4D,
        0x106C,
        0x1395,
        0x1446,
        0x124A,
        0x0DB8,
        0x06FC,
        0xFEC5,
        0xF5FE,
        0xEDB6,
        0xE70B,
        0xE2FC,
        0xE26C,
        0xE5EA,
        0xEDB6,
        0xF9B3,
        0x0958,
        0x1BC9,
        0x2FDF,
        0x443F,
        0x5776,
        0x6827,
        0x750E,
        0x7D35,
        0x7FFF,
        0x7D35,
        0x750E,
        0x6827,
        0x5776,
        0x443F,
        0x2FDF,
        0x1BC9,
        0x0958,
        0xF9B3,
        0xEDB6,
        0xE5EA,
        0xE26C,
        0xE2FC,
        0xE70B,
        0xEDB6,
        0xF5FE,
        0xFEC5,
        0x06FC,
        0x0DB8,
        0x124A,
        0x1446,
        0x1395,
        0x106C,
        0x0B4D,
        0x04E6,
        0xFE0C,
        0xF799,
        0xF258,
        0xEEE7,
        0xEDB6};

int main(void)
{	
	int n; // Working variable.

	// Intialize the board and the drivers
	DSCInit();

	// Initalize the CODEC.
	WM8510Init();

	// Start Audio input and output function
	WM8510Start();
		
	// Configure the CODEC for 8K operation	
	WM8510SampleRate8KConfig();

	for (n =0; n < SIGNAL_SIZE; n++)
	{
		signal[n] = 0;
	}

	// Use timer 1 as a real time clock.
	// 1 tick = 15,625 * 256 clock cycles = 100ms.
	timer1Init(15625);

	// Main processing loop.
	// Quit if switch 1 is pressed.
	while(SWITCH_S1)
	{
		if (dataReady)
		{
			dataReady = 0;
			// If switch 2 is pressed 
			// 		process the vectors.
			if (!SWITCH_S2)
			{
				// Shift the vector down by 1 sample.
				vectorShift(signal,SIGNAL_SIZE);

				// Put the sample at the top of the vector.
				signal[0] = signalIn; 

				// Calculate the output value.
				signalOut = vectorMAC(signal, coefficents, SIGNAL_SIZE, 4);
			
			}
			else signalOut = signalIn; // Just copy the input to the output.
		}
	} 

return(0); // Go home.
}

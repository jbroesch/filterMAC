/**********************************************************************
* © 2007 Microchip Technology Inc.
*
* FileName:        sask.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       dsPIC33FJ256GP506
* Compiler:        MPLAB® C30 v3.00 or higher
*
************************************************************************/

#include "DSCInit.h"

_FGS(GWRP_OFF & GCP_OFF);
_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE);
_FWDT(FWDTEN_OFF);

void DSCInit(void)
{
	// Configure Oscillator to operate the device at 40MHz.
	//Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
	//Fosc= 7.37M*40/(2*2)=80Mhz for 7.37M input clock */
 
	PLLFBD=41;					/* M=39	*/
	CLKDIVbits.PLLPOST=0;		/* N1=2	*/
	CLKDIVbits.PLLPRE=0;		/* N2=2	*/
	OSCTUN=0;			

	__builtin_write_OSCCONH(0x01);	//Initiate Clock Switch to FRC with PLL.
	__builtin_write_OSCCONL(0x01);
	while (OSCCONbits.COSC != 0b01); // Wait for the Clock switch to occur.
	while(!OSCCONbits.LOCK);

	/* Intialize the board LED and swicth ports	
	 * and turn all LEDS off. Also switches on the 
	 * VDD regulator on the serial flash memory 
	 * chip 	*/
	 
	long vddRegWakeUpDelay;
	
	YELLOW_LED_TRIS	= 0;
	RED_LED_TRIS = 0;		
 	GREEN_LED_TRIS = 0;	

 	YELLOW_LED = SASK_LED_OFF;	
	RED_LED	= SASK_LED_OFF;		
 	GREEN_LED = SASK_LED_OFF;		

 	SWITCH_S1_TRIS = 1;	
 	SWITCH_S2_TRIS	= 1;
 	
	VOLUME_UPDN_TRIS = 0;	
	VOLUME_CLK_TRIS	= 0;
	
	VOLUME_UPDN = 0;
	VOLUME_CLK = 0;	
	
	REGULATOR_CONTROL_ANPCFG = 1;
	REGULATOR_CONTROL_TRIS	= 0;
	REGULATOR_CONTROL_LAT = 1;

	for(vddRegWakeUpDelay = 0;
		vddRegWakeUpDelay < REGULATOR_WAKE_UP_DELAY;
			vddRegWakeUpDelay++)
				Nop();

}

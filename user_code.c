//PIC24duino code
// - using PIC24FJxxGA004/104, and GB004 family (GA002/004, GA102/104, and GB002/004 families)
// - free running timer2 for ticks, pwm and input capture
// -
// - version history
// - v2.0, 5/15/2021: simplified port
// - v2.1, 5/26/2021: implemented SystemCoreClockUpdate() - to be executed during mcu initialization or after oscillator reconfiguration
// - v2.2, 6/06/2021: implemented hardware CRC (16-bit only).
// - v2.3, 6/07/2021: revised rtcc implementation
// - v2.4, 6/16/2021: updated to support GB002/004 chips
// - v2.5, 6/19/2021: implemented output compare (dah!)
// - v2.6, 5/19/2022: output compare routines no longer advance compare registers
// - v2.7, 5/24/2022: simplified support for GA00x, GA10x, and GB00x devices
// - v2.8, 5/24/2022: support for C30 compiler introduced
//
//
//               PIC24FJ
//              |=====================|
//    Vcc       |                     |
//     |        |                Vcap |>--[.1u]-+->GND
//     |        |                     |         |
//     +-[10K]-<| MCLR        DISVreg |>--------+
//              |                     |
//              |                     |
//     +------->| OSCI            Vdd |>--+------>Vcc
//  [Xtal]      |                     | [.1u]
//     +-------<| OSCO            Vss |>--+------>GND
//              |                     |
//              |                 RP0 |>---------->Uart2TX
//              |                     |
//              |                 RB5 |>---------->LED
//              |                     |
//              |                     |
//              |                     |
//              |                     |
//              |                     |
//              |                     |
//              |                     |
//              |                     |
//              |                     |
//              |=====================|
//
//


#include "pic24duino.h"					//we use pic24duino
//#include "dhry.h"						//we use drystone benchmark

//hardware configuration
#define LED			PB5					//led pin
#define LED_DLY		(F_CPU / 2)			//half a second
//end hardware configuration

//global defines

//global variables


//user defined set up code
void setup(void) {
	pinMode(LED, OUTPUT);			//led as output pin

	//initialize the uart
	//uart1Init(UART_BR9600);			//initialize uart1
	uart2Init(UART_BR9600);			//initialize uart2

	//enable interrupts
	ei();							//enable interrupts
}

//user defined main loop
void loop(void) {
	static uint32_t tick0=0;
	uint32_t tmp0;

	if (ticks() - tick0 > LED_DLY) {
		tick0 += LED_DLY;						//advance to the next match point
		pinFlip(LED);							//digitalWrite(LED, !digitalRead(LED));	//flip led, 105 ticks

		//uart display
		//finding something to time
		tmp0=ticks();
		//digitalRead(PA5);						//read a pin, 59 ticks (base case = 29 ticks, if there is nothing)
		//uart1Init(UART_BR9600);				//initial uart, 531-29 ticks
		//digitalWrite(LED, !digitalRead(LED));	//flip led, 105 ticks
		//analogRead(ADC_VBG);
		//for (tmp=0; tmp<1000; tmp++) digitalWrite(LED, !digitalRead(LED));	//flip led, 89100/1000 ticks
		//for (tmp=0; tmp<1000; tmp++) IO_FLP(LATB, 1<<7);					//flip led, 16040/1000 ticks
		//dhrystone();							//dhrystone benchmarking
		tmp0=ticks() - tmp0;					//calculate time elapsed

		//display information
		u2Print("F_CPU=                    ", F_CPU);
		u2Print("tick0=                    ", tick0);
		u2Print("tmp0 =                    ", tmp0);
		u2Println();
	}
}

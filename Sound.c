// Port F bit 4 is negative logic switch to play sound, SW1
// Port F bit 1 is negative logic switch to play sound, SW2

#include "tm4c123gh6pm.h"
#include "Switch.h"
unsigned long Index = 0;
unsigned long triangle = 0;
unsigned long up = 63;
unsigned long square = 0;
unsigned long count = 0;
unsigned long high = 63;
unsigned char portF;
unsigned long mode;
const unsigned char SineWave[101] = {32,35,37,38,40,42,44,46,48,50,51,53,54,
																	  56,57,58,60,61,61,62,63,63,63,63,63,63,
																	  63,63,63,62,62,62,61,61,60,58,57,56,54,
		                                53,51,50,48,46,44,42,40,38,37,35,32,30,
																	  28,27,25,23,21,19,17,15,14,12,11,9,8,7,
																		5,4,4,3,2,2,1,1,1,0,1,1,1,2,2,3,4,4,5,7,
																		8,9,11,12,14,15,17,19,21,23,25,27,28,30,32};

// **************DAC_Init*********************
// Initialize 3-bit DAC 
// Input: none
// Output: none
void DAC_Init(void){
	unsigned long volatile delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // activate port A
  delay = SYSCTL_RCGC2_R;    // allow time to finish activating
  GPIO_PORTA_AMSEL_R &= ~0xFC;      // no analog 
  GPIO_PORTA_PCTL_R &= ~0xFFFFFF00; // regular function
  GPIO_PORTA_DIR_R |= 0xFC;      // make PA2-7 out
  GPIO_PORTA_AFSEL_R &= ~0xFC;   // disable alt funct on PA2-7
  GPIO_PORTA_DEN_R |= 0xFC;      // enable digital I/O on PA2-7
}

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Input: interrupt period
//        Units of period are 12.5ns
//        Maximum is 2^24-1
//        Minimum is determined by lenght of ISR
// Output: none
void Sound_Init(unsigned long period){
  DAC_Init();          
  Index = 0;
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1      
  NVIC_ST_CTRL_R = 0x0007;  // enable SysTick with core clock and interrupts
}

// **************DAC_Out*********************
// output to DAC
void DAC_Out(unsigned long data){
	data = data << 2;
  GPIO_PORTA_DATA_R = data;
}

void Interruupt_Mode(unsigned long intr_carrier){
	mode = intr_carrier;
}

// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
//  GPIO_PORTF_DATA_R ^= 0x08;     // toggle PF3, debugging green	
////*******************Sinewave
	if (mode == 2) {
		if (Index == 100)
			Index = 0;
		else
			Index = Index+1;   
		DAC_Out(SineWave[Index]); 
	}	
	
//*****************Triangle wave
	 else if(mode == 1){
		if ((up < 63) && (triangle < 63)) {
			up = up + 1;
			triangle = triangle + 1;
		}
		else if ((up == 63) && (triangle > 0)){
			triangle = triangle -1;
		}
		else if ((up == 63) && (triangle == 0)) {
			up = 0;
		}
		DAC_Out(triangle);
	}
////*****************Square wave
	else if(mode == 3){
		if ((count > 0)&&(count < 64)){
			count = count + 1;
			square = 63;
			if (count == 64)
				count = 65;
		}
		else if ((count > 64) && (count < 130)){
			square = 0;
			count = count + 1;
			if (count == 130) {
				count = 0;
				high = 63;
			}
		}
		else if ((high == 63) && (square == 0)&&(count == 0)){
			square = high;
			high = 0;
			count = 1;
		}
		DAC_Out(square);
	}
	else if((mode == 4)||(mode == 5)){
		if (Index == 100)
			Index = 0;
		else
			Index = Index+1;   
		DAC_Out(SineWave[Index]); 
	}
}

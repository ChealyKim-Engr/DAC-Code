// Port F bit 4 is negative logic switch to play sound, SW1
// SysTick ISR: PF3 ISR heartbeat

#define rPD0       (*((volatile unsigned long *)0x40007004))
#define rPD1       (*((volatile unsigned long *)0x40007008))
#define rPD2       (*((volatile unsigned long *)0x40007010))
#define rPD3       (*((volatile unsigned long *)0x40007020))
#define cPC4       (*((volatile unsigned long *)0x40006040))
#define cPC5       (*((volatile unsigned long *)0x40006080))
#define cPC6       (*((volatile unsigned long *)0x40006100))
#define cPC7       (*((volatile unsigned long *)0x40006200))

#include "tm4c123gh6pm.h"

void Keys_Init(void) {
	volatile unsigned long  delay;
  SYSCTL_RCGC2_R |= 0x0C;           // Port C, D clock
  delay = SYSCTL_RCGC2_R;           // wait 3-5 bus cycles

  GPIO_PORTD_DIR_R &= ~0x0F;        // PD0-3 input 
  GPIO_PORTD_AFSEL_R &= ~0x0F;      // not alternative
  GPIO_PORTD_AMSEL_R &= ~0x0F;      // no analog
  GPIO_PORTD_PCTL_R &= ~0x0000FFFF; // bits for PD3-PD0
	GPIO_PORTD_PUR_R = 0x0F;          // enable pull-up on PD0-3
  GPIO_PORTD_DEN_R |= 0x0F;         // enable PD3-PD0
	
  GPIO_PORTC_DIR_R |= 0xF0;         // PC4-7 output 
  GPIO_PORTC_AFSEL_R &= ~0xF0;      // not alternative
  GPIO_PORTC_AMSEL_R &= ~0xF0;      // no analog
  GPIO_PORTC_PCTL_R &= ~0xFFFF0000; // bits for PC4-PC7
  GPIO_PORTC_DEN_R |= 0xF0;         // enable PC4-PC7	
  }

//---------------------Switch_In---------------------
// bit4 PF4 SW1 switch
// bit0 PF0 SW2 switch 
unsigned long Switch_In(void){ 
  return (GPIO_PORTF_DATA_R&0x11)^0x11;
}
char ReadKey(void) {
	GPIO_PORTC_DATA_R = ~0x80; //set col_PC7 == 0;
	if 			((GPIO_PORTD_DATA_R&0x01) == 0)
		return '1';
	else if ((GPIO_PORTD_DATA_R&0x02) == 0)
		return '4'; 
	else if ((GPIO_PORTD_DATA_R&0x04) == 0)
		return '7';
	else if ((GPIO_PORTD_DATA_R&0x08) == 0)
		return '*';
	
	GPIO_PORTC_DATA_R = ~0x40; //set col_PC6 == 0;
	if 			((GPIO_PORTD_DATA_R&0x01) == 0)
		return '2';
	else if ((GPIO_PORTD_DATA_R&0x02) == 0)
		return '5';
	else if ((GPIO_PORTD_DATA_R&0x04) == 0)
		return '8';
	else if ((GPIO_PORTD_DATA_R&0x08) == 0)
		return '0';
	
	GPIO_PORTC_DATA_R = ~0x20; //set col_PC5 == 0;
	if 			((GPIO_PORTD_DATA_R&0x01) == 0)
		return '3';
	else if ((GPIO_PORTD_DATA_R&0x02) == 0)
		return '6';
	else if ((GPIO_PORTD_DATA_R&0x04) == 0)
		return '9';
	else if ((GPIO_PORTD_DATA_R&0x08) == 0)
		return '#';
		
	GPIO_PORTC_DATA_R = ~0x10; //set col_PC4 == 0;
	if 			((GPIO_PORTD_DATA_R&0x01) == 0)
		return 'A';
	else if ((GPIO_PORTD_DATA_R&0x02) == 0)
		return 'B';
	else if ((GPIO_PORTD_DATA_R&0x04) == 0)
		return 'C';
	else if ((GPIO_PORTD_DATA_R&0x08) == 0)
		return 'D';
	return 0;
}

//---------------------Delay10ms---------------------
// wait 10ms for switches to stop bouncing
void Delay10ms(void){unsigned long volatile time;
  time = 14545;  // 10msec
  while(time){
		time--;
  }
}



// Port F bit 4 is negative logic switch to play sound, SW1
// Port F bit 1 is negative logic switch to play sound, SW2

#define rPD0       (*((volatile unsigned long *)0x40007004))
#define rPD1       (*((volatile unsigned long *)0x40007008))
#define rPD2       (*((volatile unsigned long *)0x40007010))
#define rPD3       (*((volatile unsigned long *)0x40007020))
#define cPC4       (*((volatile unsigned long *)0x40006040))
#define cPC5       (*((volatile unsigned long *)0x40006080))
#define cPC6       (*((volatile unsigned long *)0x40006100))
#define cPC7       (*((volatile unsigned long *)0x40006200))

//NOTE:
//#define PF4      (*((volatile unsigned long *)0x40025040))
//#define PF3      (*((volatile unsigned long *)0x40025020))
//#define PF2      (*((volatile unsigned long *)0x40025010))
//#define PF1      (*((volatile unsigned long *)0x40025008))
//#define PF0      (*((volatile unsigned long *)0x40025004))
//#define SW1       0x10                      // on the left side of the Launchpad board
//#define SW2       0x01                      // on the right side of the Launchpad board


#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "Sound.h"
#include "Switch.h"


// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mod4
void GPIOPortF_Handler(void);
void rainMusic(unsigned long period, unsigned long delay);
// need to generate a 100 Hz sine wave
// table size is 16, so need 100Hz*16=1.6 kHz interrupt
// bus is 80MHz, so SysTick period is 80000kHz/1.6kHz = 50000

//NOTE:
// bus is 80MHz:
// SysTick period Trianglewave:80,000,000/(262Hz * 128) = 2385
// SysTick period   squarewave:80,000,000/(262Hz * 128) = 2385
// SysTick period "C" Sinewave:80,000,000/(262Hz * 101) = 3023
// SysTick period "D" Sinewave:80,000,000/(294Hz * 101) = 2694
// SysTick period "E" Sinewave:80,000,000/(330Hz * 101) = 2400
// SysTick period "F" Sinewave:80,000,000/(349Hz * 101) = 2270
// SysTick period "G" Sinewave:80,000,000/(392Hz * 101) = 2021
// SysTick period "A" Sinewave:80,000,000/(440Hz * 101) = 1800
// SysTick period "B" Sinewave:80,000,000/(494Hz * 101) = 1603

	unsigned long intr_mode;
int main(void){ 
	unsigned long input; 
	unsigned long period, delay;
  DisableInterrupts();    
  PLL_Init();          // bus clock at 80 MHz
  Switch_Init();       // Port F is onboard switches, LEDs, profiling
	Keys_Init();
	EnableInterrupts();
  intr_mode = 0;
	delay = 150;

	while(1) {
		unsigned long modee;
		modee = intr_mode;
		Interruupt_Mode(intr_mode);
		switch(modee) {
			case 1: {
				Sound_Init(2385);  // triangel wave, 262 Hz
			}
			break;
			case 2: {
				Sound_Init(3023);  // sine wave, 262 Hz
			}
			break;
			case 3: {
				Sound_Init(2385);  // square wave, 262 Hz
			}
			break;
			case 4: {
				modee = 4;
			}
			break;
			case 5: {
				modee = 5;
			}
			break;
			default: {
				modee = intr_mode;
			}
			break;		
		}		
		while (modee == intr_mode && modee != 4 && modee !=5) {
			GPIO_PORTF_DATA_R = 0x00;
			if (modee == 1)
				GPIO_PORTF_DATA_R = 0x0E;  //WHITE
			else if (modee ==2) 
				GPIO_PORTF_DATA_R = 0x06;  //PINK
			else if (modee == 3)
				GPIO_PORTF_DATA_R = 0x0C;  //SKY-BLUE
		}
//Matrix Key Program: 
	  NVIC_ST_CTRL_R = 0;	
		while(modee == 4 && intr_mode == 4){ 
			GPIO_PORTF_DATA_R = 0x00;
			input = ReadKey();   // key press == 0, negative logic		
			if ((input == '1')&&(modee == 4) && (intr_mode == 4)) {
				EnableInterrupts();
				Sound_Init(3023);   
				while((input == '1')&&(modee == 4 && intr_mode == 4)){
					GPIO_PORTF_DATA_R = 0x08;  //green
					input = ReadKey(); 
				}
				GPIO_PORTF_DATA_R = 0x00; 
	      NVIC_ST_CTRL_R = 0;	
			}	
			else if ((input == '2')&&(modee == 4) && (intr_mode == 4)){
				EnableInterrupts();
				Sound_Init(2694);  	         //Note 'D'
				while((input == '2')&&(modee == 4 && intr_mode == 4)){
					GPIO_PORTF_DATA_R = 0x02;  //RED
					input = ReadKey(); 
				}
				GPIO_PORTF_DATA_R = 0x00; 
	      NVIC_ST_CTRL_R = 0;	
			}
			else if ((input == '3')&&(modee == 4) && (intr_mode == 4)){
				EnableInterrupts();
				Sound_Init(2400);            //Note 'E'
				while((input == '3')&&(modee == 4 && intr_mode == 4)){
					GPIO_PORTF_DATA_R = 0x04;  //BLUE
					input = ReadKey(); 
				}
				GPIO_PORTF_DATA_R = 0x00; 
	      NVIC_ST_CTRL_R = 0;	
			}
			else if ((input == '4')&&(modee == 4) && (intr_mode == 4)) {
				EnableInterrupts();
				Sound_Init(2270);   				 //Note 'F'
				while((input == '4')&&(modee == 4 && intr_mode == 4)){
					GPIO_PORTF_DATA_R = 0x0E;  //WHITE
					input = ReadKey(); 
				}
				GPIO_PORTF_DATA_R = 0x00; 
		    NVIC_ST_CTRL_R = 0;	
			}
			else if((input == '5')&&(modee == 4) && (intr_mode == 4)){
				EnableInterrupts();
				Sound_Init(2021);   	       //Note 'G'
				while((input == '5')&&(modee == 4 && intr_mode == 4)){
					GPIO_PORTF_DATA_R = 0x0A;  //YELLOW
					input = ReadKey(); 
				}
				GPIO_PORTF_DATA_R = 0x00; 
			  NVIC_ST_CTRL_R = 0;	
			}
			else if((input == '6')&&(modee == 4) && (intr_mode == 4)){
				EnableInterrupts();
				Sound_Init(1800);   	       //Note 'A'
				while((input == '6')&&(modee == 4 && intr_mode == 4)){
					GPIO_PORTF_DATA_R = 0x0C;  //SKY_BLUE
					input = ReadKey(); 
				}
				GPIO_PORTF_DATA_R = 0x00; 
        NVIC_ST_CTRL_R = 0;	
			}
			else if ((input == '7')&&(modee == 4) && (intr_mode == 4)){
				EnableInterrupts();
				Sound_Init(1603);   	       //Note 'B'
				while((input == '7')&&(modee == 4 && intr_mode == 4)){
					GPIO_PORTF_DATA_R = 0x06;  //PINK
					input = ReadKey(); 
				}
				GPIO_PORTF_DATA_R = 0x00; 
				NVIC_ST_CTRL_R = 0;	
			}				
		}	
		while(modee == 5 && intr_mode == 5){
			GPIO_PORTF_DATA_R = 0x08; //GREEN
				rainMusic(period, delay);
		}
	}	  
}

void rainMusic(unsigned long period, unsigned long delay) {
		Sound_Init(2021);  //'G'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2400);	 //'E'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2021);  //'G'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2021);	 //'G'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2400);  //'E'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2021);  //'G'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2021);  //'G'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2400);  //'E'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(1800);  //'A'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2021);  //'G'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2021);  //'G'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2400);  //'E
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2270);  //'F'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2270);  //'F'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2694);  //'D'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2694);  //'D'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2270);  //'F'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2270);  //'F'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2694);  //'D'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2021);  //'G'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2270);  //'F'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2400);  //'E'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2694);  //'D'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(2400);  //'E'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(3023);  //'C'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
		Sound_Init(3023);  //'C'
		period = delay;
		while(period){ 
			Delay10ms();
			period--;
		}
}
void Switch_Init(void){  
	unsigned long volatile delay;
  SYSCTL_RCGC2_R |= 0x00000020; // (a) activate clock for port F
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTF_LOCK_R = 0x4C4F434B; // unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;         // allow changes to PF4,0
  GPIO_PORTF_DIR_R &= ~0x10;    // (c) make PF4,0 in (built-in button)
	GPIO_PORTF_DIR_R |= 0x0E; // make PF1, 2,3  out (built-in blue LED)
  GPIO_PORTF_AFSEL_R &= ~0x1F;  //     disable alt funct on PF4,0
  GPIO_PORTF_DEN_R |= 0x1F;     //     enable digital I/O on PF4,0
  GPIO_PORTF_PCTL_R &= ~0x000FFFFF; //  configure PF4,0 as GPIO
  GPIO_PORTF_AMSEL_R &= ~0x1F;  //     disable analog functionality on PF4 to 0
  GPIO_PORTF_PUR_R |= 0x10;     //     enable weak pull-up on PF4,0
  GPIO_PORTF_IS_R &= ~0x10;     // (d) PF4 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x10;    //     PF4 is not both edges
  GPIO_PORTF_IEV_R &= ~0x10;    //     PF4 falling edge event
  GPIO_PORTF_ICR_R = 0x10;      // (e) clear flags 4
  GPIO_PORTF_IM_R |= 0x10;      // (f) arm interrupt on PF4
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00400000; // (g) priority 2
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
}
void GPIOPortF_Handler(void){ 
	GPIO_PORTF_DATA_R = 0x02; 
  if(GPIO_PORTF_RIS_R&0x10){  // SW1 touch
    GPIO_PORTF_ICR_R = 0x10;  // acknowledge flag4
		if ((intr_mode == 5)||(intr_mode > 5))
			intr_mode = 1;
		else 
			intr_mode = intr_mode + 1;
  }
}

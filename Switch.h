// Port F bit 4 is negative logic switch to play sound, SW1
// Port F bit 1 is negative logic switch to play sound, SW2

//---------------------Switch_Init---------------------
// initialize switch interface
void Switch_Init(void);
void Keys_Init(void);
char ReadKey(void);

//---------------------Switch_In---------------------
// read the values of the two switches
// bit4 PF4 SW1 switch
// bit0 PF0 SW2 switch 
unsigned long Switch_In(void); 

//---------------------Delay10ms---------------------
// wait 10ms for switches to stop bouncing
void Delay10ms(void);

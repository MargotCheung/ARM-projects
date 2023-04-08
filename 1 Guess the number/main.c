//
// TMR_LED : change LED on/off by Timer1 interrupt
//
#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "Seven_Segment.h"

#include <stdlib.h>
#include "LCD.h"

volatile uint8_t KEY_Flag, release;
volatile uint32_t index_5ms, cnt_5ms, cnt_1s, cnt_100ms,index_key_scan, Key_time;
volatile uint32_t digit[4];

volatile uint32_t count_Line, count_Char;
int answer[4] = {0};
int seed = 2973;
int current_answer_position = 0, answer_completed = 0;

void TMR1_IRQHandler(void)
{	
	cnt_5ms++;
	index_5ms = cnt_5ms % 4;
	CloseSevenSegment();
	if (index_5ms == 0) {			
	ShowSevenSegment(0,digit[3]);				
	}	
	if (index_5ms == 1)  {
	ShowSevenSegment(1,digit[2]);		
	}	
	if (index_5ms == 2)  {
	ShowSevenSegment(2,digit[1]);		
	}
	if (index_5ms == 3)  {
	ShowSevenSegment(3,digit[0]);
	}		
	if (cnt_5ms % 10 == 0) 
	{
		cnt_100ms++;
		index_key_scan = cnt_100ms % 3;
		if (index_key_scan == 0)
		{
			PA0=1; PA1=1; PA2=1; PA3=1; PA4=1; PA5=0;
		}
		if (index_key_scan == 1)
		{
			PA0=1; PA1=1; PA2=1; PA3=1; PA4=0; PA5=1;
		}
		if (index_key_scan == 2)
		{
			PA0=1; PA1=1; PA2=1; PA3=0; PA4=1; PA5=1;
		}
		NVIC_EnableIRQ(GPAB_IRQn);

		if ((cnt_100ms - Key_time)>3)  { release = 1;}
	}
	
	if (cnt_5ms % 200 == 0) cnt_1s++;	

  TIMER_ClearIntFlag(TIMER1); // Clear Timer1 time-out interrupt flag
}

void GPAB_IRQHandler(void)
{
		NVIC_DisableIRQ(GPAB_IRQn);

Key_time = cnt_100ms;
release = 0;
	
if (PA->ISRC & BIT0) {        // check if PA0 interrupt occurred
		PA0=1;
	  PA->ISRC |= BIT0;         // clear PA0 interrupt status
	
if (PA3==0) { KEY_Flag =3; PA3=1;}
if (PA4==0) { KEY_Flag =6; PA4=1;}
if (PA5==0) { KEY_Flag =9; PA5=1;}
return;			
} 
if (PA->ISRC & BIT1) { // check if PA1 interrupt occurred
		PA1=1;
	  PA->ISRC |= BIT1;         // clear PA1 interrupt status  
if (PA3==0) { KEY_Flag =2; PA3=1;}
if (PA4==0) { KEY_Flag =5; PA4=1;}
if (PA5==0) { KEY_Flag =8; PA5=1;} 
return;				
} 
if (PA->ISRC & BIT2) { // check if PB14 interrupt occurred
		PA2=1;
	  PA->ISRC |= BIT2;         // clear PA interrupt status  
if (PA3==0) { KEY_Flag =1; PA3=1;}
if (PA4==0) { KEY_Flag =4; PA4=1;}
if (PA5==0) { KEY_Flag =7; PA5=1;}
return;				
}                     // else it is unexpected interrupts
PA->ISRC = PA->ISRC;	      // clear all GPB pins
}


void Init_Timer1(void)
{
  TIMER_Open(TIMER1, TIMER_PERIODIC_MODE, 200);
  TIMER_EnableInt(TIMER1);
  NVIC_EnableIRQ(TMR1_IRQn);
  TIMER_Start(TIMER1);
}

void Init_KEY(void)
{
		GPIO_SetMode(PA, (BIT0 | BIT1 | BIT2 |BIT3 | BIT4 | BIT5), GPIO_MODE_QUASI);
		GPIO_EnableInt(PA, 0, GPIO_INT_LOW);
		GPIO_EnableInt(PA, 1, GPIO_INT_LOW);
		GPIO_EnableInt(PA, 2, GPIO_INT_LOW);		
		NVIC_EnableIRQ(GPAB_IRQn);   
	  NVIC_SetPriority(GPAB_IRQn,3);
		GPIO_SET_DEBOUNCE_TIME(GPIO_DBCLKSRC_LIRC, GPIO_DBCLKSEL_256);
		GPIO_ENABLE_DEBOUNCE(PA, (BIT0 | BIT1 | BIT2));			
}

/* ------------------------------ Initialize UART settings ------------------------------------------ */

//Initialize UART settings
void Init_UART(void)
{
  UART_Open(UART0, 115200);                     // set UART0 baud rate
  UART_ENABLE_INT(UART0, UART_IER_RDA_IEN_Msk); // enable UART0 interrupt (triggerred by Read-Data-Available)
  NVIC_EnableIRQ(UART02_IRQn);		              // enable Cortex-M0 NVIC interrupt for UART02
}


void UART02_IRQHandler(void)
{
	uint8_t c;
	uint32_t u32IntSts = UART0->ISR;

	if(u32IntSts & UART_IS_RX_READY(UART0)) // check ISR on & RX is ready
  {
		while (!(UART0->FSR & UART_FSR_RX_EMPTY_Msk)){ // check RX is not empty
			
			c = UART_READ(UART0); // read UART RX data

			c = c - '0';
			// valid input(c is an integer)
			if (c != 0) {
				answer[current_answer_position++] = c;
				if (current_answer_position == 4) {
					answer_completed = 1;
					current_answer_position = 0;
				}

				//printC(8*count_Char, 16*(count_Line%4), c);
			  //if (++count_Char ==16){count_Char =0; count_Line++;}
			}
		}
		
	}
}


int main(void)
{
	int i, j, k;
	int current_line = 0;
	int a_count=0, b_count=0;
	char result[16] = {0};

  uint8_t ASSCII;
	SYS_Init();   // Intialize System/Peripheral clocks & multi-function I/Os
  Init_Timer1();
	Init_KEY();
  OpenSevenSegment();

	Init_UART();
	srand(seed);

	
	digit[3] = (rand() % 9) + 1;
	do {
		srand(cnt_5ms);
		digit[2] = (rand() % 9) + 1;
	} while (digit[2] == digit[3]);
	
	do {
		digit[1] = (rand() % 9) + 1;
	} while (digit[1] == digit[3] || digit[1] == digit[2]);
	
	do {
		digit[0] = (rand() % 9) + 1;
	} while (digit[0] == digit[3] || digit[0] == digit[2] || digit[0] == digit[1]);
	
	
	init_LCD();
	clear_LCD();

	index_5ms=cnt_5ms=cnt_1s=cnt_100ms=0;
	KEY_Flag = 0;

	//digit[3] = digit[2] =digit[1] =digit[0] =16;  
  count_Line=0; count_Char=0;

	while(1) {	
		//seed++;
		//srand(seed);
		if ((KEY_Flag !=0)&& (release ==1)) {
			
			ASSCII = KEY_Flag+48;
			KEY_Flag =0;
			UART_Write(UART0, &ASSCII, 1);
		}		
		
		//check position and value ?A?B
		if (answer_completed) {
			for (i=0; i<4; i++) {
				for (j=0; j<4; j++) {
					if (digit[i] != answer[j]) continue;
					if (i == j) a_count++;
					if (i != j) b_count++;
				}
			}
			
			//print result - 2973 4A0B
			sprintf(result, "%d%d%d%d %dA%dB", answer[0], answer[1], answer[2], answer[3], a_count, b_count);
			if (current_line < 4){ 
				print_Line(current_line++, result);
			}		
			a_count = 0;
			b_count = 0;
			answer_completed = 0;
		}

	}
}


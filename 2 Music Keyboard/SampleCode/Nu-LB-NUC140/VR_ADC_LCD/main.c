//
// LCD: display Text on LCD
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN  (LQPF-100)

#include <stdio.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "Note_Freq.h"
#include "Scankey.h"

#define  P125ms 125000
#define  P250ms 250000
#define  P500ms 500000
#define  P1S   1000000

volatile uint32_t x, u32ADCvalue,d1,d2,d3,d4;
volatile uint8_t u8ADF;	 //single end AD flag
int num;

int flag = 0 , input = 0;
char line1[16] = "                ";


/* --------------------------------------------- from ADC_VR1 folder ------------------------------------------------- */
// from ADC_VR1 folder	
void ADC_IRQHandler(void)
{
    uint32_t u32Flag;

    // Get ADC conversion finish interrupt flag
    u32Flag = ADC_GET_INT_FLAG(ADC, ADC_ADF_INT);

    if(u32Flag & ADC_ADF_INT)
        u8ADF = 1;

    ADC_CLR_INT_FLAG(ADC, u32Flag);
}

// from ADC_VR1 folder
void Init_ADC(void)
{
    ADC_Open(ADC, ADC_INPUT_MODE, ADC_OPERATION_MODE, ADC_CHANNEL_MASK);
    ADC_POWER_ON(ADC);
    ADC_EnableInt(ADC, ADC_ADF_INT);
    NVIC_EnableIRQ(ADC_IRQn);
}
	
int main(void)
{
	uint8_t i;

/* Lab 9.1 music 
  int music[72] = {
	E6 ,D6u,E6 ,D6u,E6 ,B5 ,D6 ,C6 ,A5 ,A5 , 0 , 0 ,
	C5 ,E5 ,A5 ,B5 ,B5 , 0 ,C5 ,A5 ,B5 ,C6 ,C6 , 0 ,
	E6 ,D6u,E6 ,D6u,E6 ,B5 ,D6 ,C6 ,A5 ,A5 , 0 , 0 ,
	C5 ,E5 ,A5 ,B5 ,B5 , 0 ,E5 ,C6 ,B5 ,A5 ,A5 , 0 ,
	B5 ,C6 ,D6 ,E6 ,E6 , 0 ,G5 ,F6 ,E6 ,D6 ,D6 , 0 ,
	F5 ,E6 ,D6 ,C6 ,C6 , 0 ,E5 ,D6 ,C6 ,B5 ,B5 , 0 };
	
  uint32_t pitch[72] = {
	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,
	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,
	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,	P250ms, P250ms, P250ms, P250ms,
	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms,
	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms,
	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms,	P500ms, P500ms, P500ms, P500ms
	};
	//.
*/

	//Initialise functions
	SYS_Init();
	init_LCD();
	clear_LCD();
	Init_ADC(); 

	OpenKeyPad();  
	
  u8ADF = 0;

/* ------------------------- Change PWM0 to PWM1 -------------------------------------- */	
	//PWM1 = PA4 ~ PA7 to activate LED light	
	PWM_EnableOutput(PWM1, PWM_CH_0_MASK);
  	PWM_Start(PWM1, PWM_CH_0_MASK);	
	
	/*
	sprintf(line1,"Music : ");
	print_Line(0,line1);
	*/
	
    while(1) {

		input = ScanKey();
			
      ADC_START_CONV(ADC);
      while (u8ADF == 0);
      u32ADCvalue = ADC_GET_CONVERSION_DATA(ADC, 7);
			x = u32ADCvalue;
			//x = (int) ((((float) u32ADCvalue )/4096.0)* 3.3 * 1000.0);
			PWM_EnableOutput(PWM1, PWM_CH_0_MASK);
				
			
		switch(input)
		{
			case 0:
				PWM_DisableOutput(PWM1, PWM_CH_0_MASK);
				if(flag == 1)
				{					
					flag = 0;
				}					
			break;

			case 1:
				if(!flag)
				{
					PWM_EnableOutput(PWM1, PWM_CH_0_MASK);
					//function(PWM1[PA12], channel, PWM frequency, duty ratio)
					PWM_ConfigOutputChannel(PWM1, PWM_CH0, C4, 90);
					sprintf(line1,"Music : C4");
					flag = 1;
					break;
				}

			case 2:
				if(!flag)
				{
					PWM_EnableOutput(PWM1, PWM_CH_0_MASK);
					PWM_ConfigOutputChannel(PWM1, PWM_CH0, D4, 90);
					sprintf(line1,"Music : D4");
					flag = 1;
					break;
				}

			case 3:
				if(!flag)
				{
					PWM_EnableOutput(PWM1, PWM_CH_0_MASK);
					PWM_ConfigOutputChannel(PWM1, PWM_CH0, E4, 90);
					sprintf(line1,"Music : E4");
					flag = 1;
					break;
				}

			case 4:
				if(!flag)
				{
					PWM_EnableOutput(PWM1, PWM_CH_0_MASK);
					PWM_ConfigOutputChannel(PWM1, PWM_CH0, F4, 90);
					sprintf(line1,"Music : F4");
					flag = 1;
					break;
				}

			case 5:
				if(!flag)
				{
					PWM_EnableOutput(PWM1, PWM_CH_0_MASK);
					PWM_ConfigOutputChannel(PWM1, PWM_CH0, G4, 90);
					sprintf(line1,"Music : G4");
					flag = 1;
					break;
				}

			case 6:
				if(!flag)
				{
					PWM_EnableOutput(PWM1, PWM_CH_0_MASK);
					PWM_ConfigOutputChannel(PWM1, PWM_CH0, A4, 90);
					sprintf(line1,"Music : A4");
					flag = 1;
					break;
				}

			case 7:
				if(!flag)
				{
					PWM_EnableOutput(PWM1, PWM_CH_0_MASK);
					PWM_ConfigOutputChannel(PWM1, PWM_CH0, B4, 90);
					sprintf(line1,"Music : B4");
					flag = 1;
					break;
				}

			case 8:
				if(!flag)
				{
					PWM_EnableOutput(PWM1, PWM_CH_0_MASK);
					PWM_ConfigOutputChannel(PWM1, PWM_CH0, C5, 90);
					sprintf(line1,"Music : C5");
					flag = 1;
					break;
				}

			case 9:
				if(!flag)
				{
					PWM_EnableOutput(PWM1, PWM_CH_0_MASK);
					PWM_ConfigOutputChannel(PWM1, PWM_CH0, D5, 90);
					sprintf(line1,"Music : D5");
					flag = 1;
					break;
				}
			}

      ADC_DisableInt(ADC, ADC_ADF_INT);

			//Display the scale on LCD screen
			print_Line(0,line1);
    }	
}

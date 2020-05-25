#include <stdint.h>
#include <stdbool.h>
#include "switch_counter_interrupt_TivaWare.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"


#include "Tivastuff.h"
#include "driverlib/rom_map.h"


#define RED 0x02
#define BLUE 0x04
#define GREEN 0x08


//*****************************************************************************
//
//!
//! Design a counter. The counter is incremented by 1 when SW1 (PF4) or SW2 (PF0) 
//! is pressed.
//
//*****************************************************************************

// global variable visible in Watch window of debugger
// increments at least once per button press
int count = 0;

void
PortFunctionInit(void)
{
    //
    // Enable Peripheral Clocks 
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	
	// PWM PD0, motor 1
  GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_6);
  GPIOPinConfigure(GPIO_PD0_M1PWM0);
	
	// PWM PD0, motor 2
//GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
//GPIOPinConfigure(GPIO_PD0_M1PWM0
	
		// PWM PD0, motor 3
//GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
//GPIOPinConfigure(GPIO_PD0_M1PWM0);

    //
    // Enable pin PA7 for GPIOOutput
    //
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_7);

    //
    // Enable pin PA2 for GPIOOutput
    //
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2);

    //
    // Enable pin PA3 for GPIOOutput
    //
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);

    //
    // Enable pin PA6 for GPIOOutput
    //
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6);

    //
    // Enable pin PA5 for GPIOOutput
    //
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5);

    //
    // Enable pin PA4 for GPIOOutput
    //
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);

    //
    // Enable pin PC7 for GPIOOutput
    //
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7);

    //
    // Enable pin PC6 for GPIOOutput
    //
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6);

    //
    // Enable pin PC5 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_5);

    //
    // Enable pin PC4 for GPIOOutput
    //
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);

    //
    // Enable pin PF3 for GPIOOutput
    //
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

    //
    // Enable pin PF4 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);

    //
    // Enable pin PF1 for GPIOOutput
    //
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);

    //
    // Enable pin PF2 for GPIOOutput
    //
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

    //
    // Enable pin PF0 for GPIOInput
    //

    //
    //First open the lock and select the bits we want to modify in the GPIO commit register.
    //
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0x1;

    //
    //Now modify the configuration of the pins that we unlocked.
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);

		//Enable pull-up on PF4 and PF0
		GPIO_PORTF_PUR_R |= 0x11; 

}


void
Interrupt_Init(void)
{
  IntEnable(INT_GPIOF);  							// enable interrupt 30 in NVIC (GPIOF)
	IntPrioritySet(INT_GPIOF, 0x00); 		// configure GPIOF interrupt priority as 0
	GPIO_PORTF_IM_R |= 0x11;   		// arm interrupt on PF0 and PF4
	GPIO_PORTF_IS_R &= ~0x11;     // PF0 and PF4 are edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;   	// PF0 and PF4 not both edges trigger 
  GPIO_PORTF_IEV_R &= ~0x11;  	// PF0 and PF4 falling edge event
	IntMasterEnable();       		// globally enable interrupt
	
}

//interrupt handler //[need for og code]   [need to eddit in startup file to un-comment for GPIO port F]
void GPIOPortF_Handler(void)
{
	
	//SW1 is pressed
	if(GPIO_PORTF_RIS_R&0x10)
	{
		// acknowledge flag for PF4
		GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_4); 
		//counter imcremented by 1
		count++;
	}
	
	//SW2 is pressed
  if(GPIO_PORTF_RIS_R&0x01)
	{
		// acknowledge flag for PF0
		GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);
		//counter imcremented by 1
		count--;
	}
}

//void Number(){
//	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00); //Set all LEDs to Off
//	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
//	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
//	if(count == 0){
//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, RED); //Toggle Red LED 
//		SysCtlDelay(2000000);
//	}else if (count == 1){
//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, BLUE); //Toggle Blue LED
//		SysCtlDelay(2000000);
//	}else if (count ==2){
//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GREEN);//Toggle Green LED 
//		SysCtlDelay(2000000);
//	}else if(count>2){
//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GREEN); //Rest Yellow (Too High)
//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, RED);
//	}else if (count<0){
//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GREEN); //Rest Cyan (Too Low)
//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, BLUE);
//		
//	}
//}

int main(void)
{
//		bool SW1 = false; //defining boolean values
//		bool SW2 = false; 
		//initialize the GPIO ports	
		PortFunctionInit();
		
	//configure the GPIOF interrupt
		Interrupt_Init();
	
    //
    // Loop forever.
    //
    while(1)
    {
//			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
//			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00);
//			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00
			
			if(GPIO_PORTF_RIS_R&0x10)
			{
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0x10); //Start Motor 1
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0x10);
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0x00);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, BLUE); //Blue light test
		SysCtlDelay(6000000);
		
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0x10); //Pulse Motor 2
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0x10);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00); 
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GREEN); //Green light test		
		SysCtlDelay(4000000);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0x00);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00); //Green light test	
		
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x10); //Start Motor 3 Cycle
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x10); //Move front to back
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0x00); 
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, RED); //Red light test	
		SysCtlDelay(10000000);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00); //Move back to front
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0x10);
		SysCtlDelay(10000000);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x10); //Move front to back
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0x00);
		SysCtlDelay(10000000);
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0x00); //Move back to front
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0x10);
		SysCtlDelay(10000000);
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0x00); //Start Motor 3 Cycle
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00); //Red light test
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0x00); //Stop Motor 1
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0x00); //Blue light test
		
	}
			}
	}

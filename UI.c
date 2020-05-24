#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "..\Final Project\LCD library\LCD.h"
#include "inc\hw_types.h"
#include "inc\hw_memmap.h"
#include "inc\hw_gpio.h"
#include "inc\tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"

void PortFunctionInit(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
  HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0x1;
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
	GPIO_PORTF_PUR_R |= 0x11;
}

void Pushed(void){
	if((GPIO_PORTF_DATA_R&0x01)==0x00){
			LCD_Print("You have chosen", "option 2 ");
			SysCtlDelay(100000000/3); //Delay
			LCD_Clear();
			//rest of the team's code functions called here
			LCD_Print("Thank you for", "visiting.");
		SysCtlDelay(100000000/3); //Delay
			LCD_Clear();
			exit(1);
		}else if((GPIO_PORTF_DATA_R&0x10)==0x00){
			LCD_Print("You have chosen", "option 1 ");
			SysCtlDelay(100000000/3); //Delay
			LCD_Clear();
			//rest of the team's code functions called here
			LCD_Print("Thank you for", "visiting.");
			SysCtlDelay(100000000/3); //Delay
			LCD_Clear();
			exit(2);
		}
}

int main(void) {
				bool on = false;

        SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

        LCD_init();
				
				PortFunctionInit();
				
				LCD_Clear();
				while(1){
					if((GPIO_PORTF_DATA_R&0x10)==0x00 && (GPIO_PORTF_DATA_R&0x01)==0x00){
						on = true;
						while(on == true){
            LCD_Print("Welcome to the", "car wash"); //Print 2 lines
            SysCtlDelay(100000000/3); //Delay
            LCD_Clear();
						Pushed();

            LCD_Print("Please select", "an option below");
            SysCtlDelay(40000000/3); //Delay
						LCD_Clear();
						Pushed();

            LCD_Print("1. Standard, $7", "2. Deluxe, $10 ");
            SysCtlDelay(40000000/3); //Delay
						LCD_Clear();
						Pushed();
					
						LCD_Print("Use SW1 to", "select 1 ");
            SysCtlDelay(40000000/3); //Delay
            LCD_Clear();
						Pushed();
					
						LCD_Print("Use SW2 to", "select 2 ");
            SysCtlDelay(40000000/3); //Delay
            LCD_Clear();
						Pushed();
					
						LCD_Print("SW1", "<--------");
            SysCtlDelay(40000000/3); //Delay
            LCD_Clear();
						Pushed();
						
						LCD_Print("SW2", "-------->");
            SysCtlDelay(40000000/3); //Delay
            LCD_Clear();
						Pushed();
					}
				}
			}
}

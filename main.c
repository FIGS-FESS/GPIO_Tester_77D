/*
 * Includes
 */
#include "F28x_Project.h"

/*
 * Globals
 */

uint32_t pin_states[6] = {0x0,0x0,0x0,0x0,0x0,0x0};
uint32_t pin_mask[6];
int GPIO_Test_Pin = 0;
int Last_Pin = 0;

/*
 * Enums
 */

//Gpio Register Enumerations
typedef enum Gpio_Regs{
    Gpio_A,
    Gpio_B,
    Gpio_C,
    Gpio_D,
    Gpio_E,
    Gpio_F
}Gpio_Regs;

/*
 * Function Prototypes
 */

void Coil_Gpio_Init();
void Gpio_Output();

/*
 * main.c
 */

void main(void) {
    //Main Initialization function for the 77D
    InitSysCtrl();

    //Setup the GPIO pins
    InitGpio();

    GPIO_SetupPinOptions(42,1,0);
	Coil_Gpio_Init();
    for(;;){
        if(GPIO_Test_Pin != Last_Pin){
            pin_states[(Last_Pin >> 5) & 0x07] = 0x00000000;
            Last_Pin = GPIO_Test_Pin;
            pin_states[(GPIO_Test_Pin>>5)&0x7] = (uint32_t)1 << (GPIO_Test_Pin & 0x001F);
            Gpio_Output();
        }
    }
}

/*
 * Initialization functions for the GPIO pins for the coils
 */

void Coil_Gpio_Init(){
    pin_mask[Gpio_A] = 0xFFFFFFFF;  //Init variable for GPIO A Registers
    pin_mask[Gpio_B] = 0xFFFFFFFF;  //Init variable for GPIO B Registers
    pin_mask[Gpio_C] = 0xFFFFFFFF;  //Init variable for GPIO C Registers
    pin_mask[Gpio_D] = 0xFFFFFFFF;  //Init variable for GPIO D Registers
    pin_mask[Gpio_E] = 0xFFFFFFFF;  //Init variable for GPIO E Registers
    pin_mask[Gpio_F] = 0x000001FF;  //Init variable for GPIO F Registers

    EALLOW;

    //Sets CPU1 to control all GPIO pins
    GpioCtrlRegs.GPACSEL1.all = 0x00000000;
    GpioCtrlRegs.GPACSEL2.all = 0x00000000;
    GpioCtrlRegs.GPACSEL3.all = 0x00000000;
    GpioCtrlRegs.GPACSEL4.all = 0x00000000;
    GpioCtrlRegs.GPBCSEL1.all = 0x00000000;
    GpioCtrlRegs.GPBCSEL2.all = 0x00000000;
    GpioCtrlRegs.GPBCSEL3.all = 0x00000000;
    GpioCtrlRegs.GPBCSEL4.all = 0x00000000;
    GpioCtrlRegs.GPCCSEL1.all = 0x00000000;
    GpioCtrlRegs.GPCCSEL2.all = 0x00000000;
    GpioCtrlRegs.GPCCSEL3.all = 0x00000000;
    GpioCtrlRegs.GPCCSEL4.all = 0x00000000;
    GpioCtrlRegs.GPDCSEL1.all = 0x00000000;
    GpioCtrlRegs.GPDCSEL2.all = 0x00000000;
    GpioCtrlRegs.GPDCSEL3.all = 0x00000000;
    GpioCtrlRegs.GPDCSEL4.all = 0x00000000;
    GpioCtrlRegs.GPECSEL1.all = 0x00000000;
    GpioCtrlRegs.GPECSEL2.all = 0x00000000;
    GpioCtrlRegs.GPECSEL3.all = 0x00000000;
    GpioCtrlRegs.GPECSEL4.all = 0x00000000;
    GpioCtrlRegs.GPFCSEL1.all = 0x00000000;
    GpioCtrlRegs.GPFCSEL2.all = 0x00000000;

    //Fix for pins 93 and 95
    GpioCtrlRegs.GPBAMSEL.all = 0x00000000;

    //Sets the Drain to Normal Output
    GpioCtrlRegs.GPAODR.all = 0x00000000;
    GpioCtrlRegs.GPBODR.all = 0x00000000;
    GpioCtrlRegs.GPCODR.all = 0x00000000;
    GpioCtrlRegs.GPDODR.all = 0x00000000;
    GpioCtrlRegs.GPEODR.all = 0x00000000;
    GpioCtrlRegs.GPFODR.all = 0x00000000;

    //Set the MUX for the pins
    GpioCtrlRegs.GPAMUX1.all = 0x00000000;
    GpioCtrlRegs.GPAMUX2.all = 0x00000000;
    GpioCtrlRegs.GPBMUX1.all = 0x00000000;
    GpioCtrlRegs.GPBMUX2.all = 0x00000000;
    GpioCtrlRegs.GPCMUX1.all = 0x00000000;
    GpioCtrlRegs.GPCMUX2.all = 0x00000000;
    GpioCtrlRegs.GPDMUX1.all = 0x00000000;
    GpioCtrlRegs.GPDMUX2.all = 0x00000000;
    GpioCtrlRegs.GPEMUX1.all = 0x00000000;
    GpioCtrlRegs.GPEMUX2.all = 0x00000000;
    GpioCtrlRegs.GPFMUX1.all = 0x00000000;

    //Set the Direction on each of the PWM_H, PWM_L and Dir pins
    GpioCtrlRegs.GPADIR.all |= pin_mask[Gpio_A];     //Sets the selected GPIO A pins to output
    GpioCtrlRegs.GPBDIR.all |= pin_mask[Gpio_B];     //Sets the selected GPIO B pins to output
    GpioCtrlRegs.GPCDIR.all |= pin_mask[Gpio_C];     //Sets the selected GPIO C pins to output
    GpioCtrlRegs.GPDDIR.all |= pin_mask[Gpio_D];     //Sets the selected GPIO D pins to output
    GpioCtrlRegs.GPEDIR.all |= pin_mask[Gpio_E];     //Sets the selected GPIO E pins to output
    GpioCtrlRegs.GPFDIR.all |= pin_mask[Gpio_F];     //Sets the selected GPIO F pins to output

    //Sets the Output low on each of th PWM_H, PWM_L and Dir pins
    GpioDataRegs.GPACLEAR.all = pin_mask[Gpio_A];     //Sets the selected GPIO A pins low
    GpioDataRegs.GPBCLEAR.all = pin_mask[Gpio_B];     //Sets the selected GPIO B pins low
    GpioDataRegs.GPCCLEAR.all = pin_mask[Gpio_C];     //Sets the selected GPIO C pins low
    GpioDataRegs.GPDCLEAR.all = pin_mask[Gpio_D];     //Sets the selected GPIO D pins low
    GpioDataRegs.GPECLEAR.all = pin_mask[Gpio_E];     //Sets the selected GPIO E pins low
    GpioDataRegs.GPFCLEAR.all = pin_mask[Gpio_F];     //Sets the selected GPIO F pins low

    EDIS;
}

/*
 * GPIO Set/Clear Function
 */

void Gpio_Output(){
    GpioDataRegs.GPACLEAR.all = ~pin_states[Gpio_A];    //Sets the selected GPIO A pins low
    GpioDataRegs.GPBCLEAR.all = ~pin_states[Gpio_B];    //Sets the selected GPIO B pins low
    GpioDataRegs.GPCCLEAR.all = ~pin_states[Gpio_C];    //Sets the selected GPIO C pins low
    GpioDataRegs.GPDCLEAR.all = ~pin_states[Gpio_D];    //Sets the selected GPIO D pins low
    GpioDataRegs.GPECLEAR.all = ~pin_states[Gpio_E];    //Sets the selected GPIO E pins low
    GpioDataRegs.GPFCLEAR.all = (~pin_states[Gpio_F]) & pin_mask[Gpio_F];    //Sets the selected GPIO E pins low

    GpioDataRegs.GPASET.all = pin_states[Gpio_A];    //Sets the selected GPIO A pins High
    GpioDataRegs.GPBSET.all = pin_states[Gpio_B];    //Sets the selected GPIO B pins High
    GpioDataRegs.GPCSET.all = pin_states[Gpio_C];    //Sets the selected GPIO C pins High
    GpioDataRegs.GPDSET.all = pin_states[Gpio_D];    //Sets the selected GPIO D pins High
    GpioDataRegs.GPESET.all = pin_states[Gpio_E];    //Sets the selected GPIO E pins High
    GpioDataRegs.GPFSET.all = pin_states[Gpio_F] & pin_mask[Gpio_F];    //Sets the selected GPIO E pins High
}


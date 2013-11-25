#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_
// Digital Output
#define RGB0_RED    13 /*Red PWM*/
#define RGB0_GREEN  11 /*Green PWM*/
#define RGB0_BLUE   10 /*Blue PWM*/

#define LED0       9 /*Green PWM*/
#define LED1       12
#define LED2       1
#define LED3       6 /*PWM*/
#define LED4       5 /*PWM*/

#define BUZZER0  0 /*PWM*/

// Digital Input
#define BUTTON0    3  /*Interrupt capable*/
#define BUTTON1    2  /*Interrupt capable*/
#define BUTTON2    A4 /*Analog A4 is being stolen for digital usage */
#define BUTTON3    4

#define SWITCH0    7
#define SWITCH1    8

/* Analog In */
#define PHOTO_RESISTOR0  A5
#define POTENTIOMETER0   A0
#define ACCEL0_X         A1
#define ACCEL0_Y         A2
#define ACCEL0_Z         A3

#define POT_FILTER       1
#define LIGHT_FILTER     4
#define ACCEL_FILTER     8
#define TEMP_FILTER      0

typedef struct
{
  int a;
  int v;
}AddressValuePair;

#endif /* _CONTROLLER_H*/

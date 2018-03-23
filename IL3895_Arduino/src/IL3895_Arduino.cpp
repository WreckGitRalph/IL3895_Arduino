/*
 * Port from STM32 to Arduino Uno of the EPAPER initialization example from GoodDisplay.
 * Created on March 17, 2018, 5:16 PM
 */

#include <Arduino.h>
#include "screen.h"
#include "IL3895_Arduino.h"

/*
 * IL3895_init
 * Initialize the display
 * Future: take display model as argument, set config appropriately
 */
void IL3895_init(void){
    EpaperIO_Init(); 
    Epaper_Init(); 
    LUT_Written_by_MCU();
}

/*
 * IL3895_test
 * Put driver into test mode, toggling screen on and off indefinitely
 */
void IL3895_test(void){
  while(1)
  {
    Fill_Screen(1);  
    delay(500);                                                      
    Fill_Screen(0);
    delay(500);
  }
}

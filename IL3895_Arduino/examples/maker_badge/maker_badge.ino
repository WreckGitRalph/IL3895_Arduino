//An exaple sketch using the IL3895_Arduino arduino library.
//
//This sketch can be used with an Arduino UNO, but was developed for a bare Atmega168, as defined 
//in the KiCAD schematic at:
//https://github.com/WreckGitRalph/EPD_Card
//

#include <avr/sleep.h>
#include <CapacitiveSensor.h>
#include <IL3895_Arduino.h>
#include "images.h"
#include <SPI.h>

#define __AVR_ATmega168P__

CapacitiveSensor button1 = CapacitiveSensor(0,1);
uint8_t pageNum;
uint8_t wdt_flag = 0;
long touch1;

/*
 * Name: ISR(WDT_vect)
 * Internal watchdog interrupt. Runs every second to wake the AVR from sleep.
 */
 ISR(WDT_vect){
    wdt_flag=1;
 }

/*
 * Name: WDT_init()
 * Initializes the watchdog timer interrupt
 */
void WDT_init(void){
  MCUSR &= ~(1<<WDRF);              //clear the reset flag, just in case
  cli();                            //disable interrupts
  WDTCSR |= (1<<WDCE) | (1<<WDE);   //put WDT in edit mode
  WDTCSR = 1<<WDP1 | 1<<WDP2;       //set timeout to 1s
  WDTCSR |= _BV(WDIE);              //set WDT to interrupt mode
  sei();                            //enable interrupts
}

void setup()
{

pinMode(5,OUTPUT);

  IL3895_init(HORIZONTAL);
  button1.reset_CS_AutoCal();
  
  pageNum=1;

     Draw_Rectangle(0,151,0,151,WHITE);
     Draw_Image_Fullscreen(LinkedInQR);
     
     Epaper_Update();

  WDT_init();

}

void loop()
{

  //after each wake, check if the button is being touched and switch the page
  if (wdt_flag==1){
    touch1 = button1.capacitiveSensor(50);

    if((touch1>90)&&(touch1<200)){
      
      switch(pageNum) {
        case 0:
             Draw_Rectangle(0,151,0,151,WHITE);
             Draw_Image_Fullscreen(LinkedInQR);
       
             Epaper_Update();
             delay(1000);
             pageNum=1;
             break;
        case 1:
             Draw_Rectangle(0,151,0,151,WHITE);
             Write_String(20,140,BLACK,"I.T. PROFESSIONAL");
             Write_String(60,140,BLACK,"INVENTOR,");
             Write_String(100,140,BLACK,"MAKER");
        
             Epaper_Update();
             delay(1000);
             pageNum=2;
             break;

        case 2:
            Draw_Rectangle(0,151,0,151,WHITE);
            Write_String(04,145,BLACK,"Tel:");
            Write_String(20,151,BLACK,"(123) 456-7890");
            Write_String(54,145,BLACK,"Email:");
            Write_String(70,151,BLACK,"your@email.com");
            Write_String(104,145,BLACK,"LinkedIn:");
            Write_String(120,151,BLACK,"www.linkedin.com/in/yourlinkedinpage");

             Epaper_Update();
             delay(1000);
             pageNum=0;
             break;
      }
  
    }
    wdt_flag=0;
  }

  //go to sleep and wait for watchdog interrupt
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  
  //...
  
  sleep_disable(); 

}



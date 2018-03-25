/*
 * Port from STM32 to Arduino Uno of the EPAPER initialization example from GoodDisplay.
 * Created on March 17, 2018, 5:16 PM
 */

#include <Arduino.h>
#include <SPI.h>
#include "screen.h"
#include "IL3895_Arduino.h"
#include "font.h"

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
* Function Name: Write_Char
 * Description : write a single 8x8 character at the given coordinates 
 * Input : x - x-coordinate of the top left corner
 *          y - y-coordinate of the top left corner
 *          charByte - ASCII character to display
 * Output : None
 */
void Write_Char(uint16_t x, uint16_t y, uint8_t colour, uint8_t charByte){
    
    uint8_t charHeight=(sizeof(font8x11[0]))/(sizeof(font8x11[0][0]));
            
    //wrap if x is off the screen
    while(x>(151-8)){
        y+=charHeight;
        x-=151;
    }
    if((y>151)||(charByte>128)){
        return;
    }
    
    //set write window and counter
    Set_Write_Window(x, x, y, (y+charHeight));
    
    //fonts start at ASCII 33
    charByte-=32;
    
    Epaper_Write_Command(CMD_WRITE_RAM);
    uint8_t i;
    for(i=0;i<charHeight;i++){
         
        uint8_t printChar;
        if(colour==BLACK){
            printChar=~(font8x11[charByte][i]);
        }else{
            uint8_t printChar=font8x11[charByte][i];
        }

        Epaper_Write_Data(printChar);
        
    }
    Epaper_READBUSY();
}


/*
 * IL3895_test
 * Put driver into test mode indefinitely
 */
void IL3895_test(void){
    
  char testString[] = "This Is A Test";
  
  while(1)
  { 
    
    uint8_t x=0;
    uint8_t y=0;
    int i;
    int j;
    for(i=0;i<(sizeof(testString)/sizeof(testString[0]))-1;i++){
        
        Write_Char(x,y,BLACK,testString[i]);
        x+=8;
    }
    Epaper_Update();
    delay(500);
     
    //fill screen with grey
    Fill_Screen(2);  
    delay(500);   

    //draw some squares
    Set_Write_Window(32,96,32,96);
    
    Epaper_Write_Command(CMD_WRITE_RAM);   
    for(i=0;i<64;i++){
        for(j=0;j<(64/8);j++){
            Epaper_Write_Data(0x00);
        }
    }
    
    Set_Write_Window(64,128,64,128);
    
    Epaper_Write_Command(CMD_WRITE_RAM);   
    for(i=0;i<=64;i++){
        for(j=0;j<=(64/8);j++){
            Epaper_Write_Data(0x00);
        }
    }
 
    Epaper_Update();
 
    Fill_Screen(1);  
    delay(500);
    
    Fill_Screen(0);
    delay(500);
  }
}

/*
 * Marcus Niekraszewicz
 * Graphics and text functions for GoodDisplay IL3895 based ePaper displays
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
 * Description : write a single character at the given coordinates 
 * Input : x - x-coordinate of the top left corner
 *          y - y-coordinate of the top left corner
 *          charByte - ASCII character to display
 * Output : None
 */
void Write_Char(uint16_t *x, uint16_t *y, uint8_t colour, uint8_t inChar){
    
    uint8_t charLen=sizeof(font8x11[0][0]);
    uint8_t charHeight=(sizeof(font8x11[0]))/charLen;
            
    //wrap if x is off the screen
    while(*x>(151-8)){
        *y+=charHeight;
        *x-=151;
    }
    if((*y>151)||(32>inChar>126)){
        return;
    }
    
    //set write window and counter
    Set_Write_Window(*x, *x, *y, (*y+charHeight));
    
    //fonts start at ASCII 33
    inChar-=32;
    
    Epaper_Write_Command(CMD_WRITE_RAM);
    uint8_t i;
    for(i=0;i<charHeight;i++){
         
        uint8_t printChar;
        if(colour==BLACK){
            printChar=~(font8x11[inChar][i]);
        }else{
            printChar=font8x11[inChar][i];
        }

        Epaper_Write_Data(printChar);
        
    }

    *x+=(charLen*8);

}

/*
* Function Name: Write_String
 * Description : write a string starting at the given coordinates 
 * Input : x - x-coordinate of the top left corner
 *          y - y-coordinate of the top left corner
 *          inString - ASCII string to write
 * Output : None
 */
void Write_String(uint16_t x, uint16_t y, uint8_t colour, char inString[]){
    
    uint8_t i;
    
    for(i=0;inString[i]!=0;i++){
        Write_Char(&x, &y, colour, inString[i]);
    }
}

/*
* Function Name: Draw rectangle
 * Description : Draw a rectangle of a defined size at the given coordinates 
 * Input : x - x1- starting x coordinate of the rectangle
 *              x2- ending x coordinate of the rectangle
 *              y1- starting y coordinate of the rectangle
 *              y2- ending y coordinate of the rectangle
 *          colour - fill colour of the rectangle
 * Output : None
 */
void Draw_Rectangle(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2, uint8_t colour){
    
    Set_Write_Window(x1,x2,y1,y2);
    
    uint16_t i;
    uint16_t j;
    Epaper_Write_Command(CMD_WRITE_RAM);   
    for(i=0;i<=(y2-y1);i++){
        for(j=0;j<=((x2-x1)/8);j++){
            Epaper_Write_Data(colour);
        }
    }
 
}

/*
 * IL3895_test
 * Put driver into test mode indefinitely
 */
void IL3895_test(void){
    
  //char testString[] = "This Is A Test";
  
  while(1)
  { 
    
    int i;
    int j;
      
    Write_String(32, 64, BLACK, "This Is A Test");
    Epaper_Update();
    delay(500);
     
    //fill screen with grey
    Fill_Screen(GREY);  
    delay(500);   

    //draw some squares
    Draw_Rectangle(32,96,32,96,WHITE);
    
 /*   Epaper_Write_Command(CMD_WRITE_RAM);   
    for(i=0;i<64;i++){
        for(j=0;j<(64/8);j++){
            Epaper_Write_Data(0x00);
        }
    }
 */
    
    Draw_Rectangle(64,128,64,128,BLACK);
    
/*    Epaper_Write_Command(CMD_WRITE_RAM);   
    for(i=0;i<=64;i++){
        for(j=0;j<=(64/8);j++){
            Epaper_Write_Data(0x00);
        }
    }
 */
 
    Epaper_Update();
 
    Fill_Screen(BLACK);  
    delay(500);
    
    Fill_Screen(WHITE);
    delay(500);
  }
}

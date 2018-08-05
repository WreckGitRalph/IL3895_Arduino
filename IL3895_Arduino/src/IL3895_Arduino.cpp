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
#include <avr/pgmspace.h>

enum orientation global_orientation;

/*
 * IL3895_init
 * Initialize the display
 * Future: take display model as argument, set config appropriately
 */
void IL3895_init(enum orientation init_orientation){
    EpaperIO_Init(); 
    Epaper_Init(); 
    LUT_Written_by_MCU();
    global_orientation=init_orientation;
}

/*
* Function Name: Write_Char
 * Description : write a single character at the given coordinates 
 * Input : x - x-coordinate of the top left corner, in bits
 *          y - y-coordinate of the top left corner, in bits
 *          charByte - ASCII character to display
 * Output : None
 */
void Write_Char(uint16_t *x, uint16_t *y, uint8_t colour, uint8_t inChar){
   
    Serial.print(inChar);
    Serial.print(":");
    

    //fonts start at ASCII 32
    inChar-=32;
    uint8_t charIndex;	//index to font entry in fontDescriptors array
    uint8_t charLen;	//character width on a vertially oriented display, in bytes
    uint8_t charHeight;	//character height on a vertically oriented display, in bits
    uint16_t charOffset;	//offset in font array

    switch (global_orientation){
	    case VERTICAL:
		charIndex=inChar;
		charLen=(((pgm_read_byte(&fontDescriptors[inChar].charWidth))-1)/8)+1; 
    		charHeight=pgm_read_byte(&fontDescriptors[inChar].charHeight);
		charOffset=pgm_read_word(&fontDescriptors[inChar].offset);
		break;
	    case HORIZONTAL:
		charIndex=inChar+(fontInfo.endCharacter)-32;
		charLen=(((pgm_read_byte(&fontDescriptors[charIndex].charHeight))-1)/8)+1;	
		charHeight=pgm_read_byte(&fontDescriptors[charIndex].charWidth);
		uint16_t addOffset=(pgm_read_word(&fontDescriptors[(fontInfo.endCharacter)-32].offset))+15;	
		charOffset=(pgm_read_word(&fontDescriptors[charIndex].offset))+addOffset;
		break;
    }

    Serial.print(charIndex);
    Serial.print("\n");

	
    //wrap if x is off the screen
    while(*x>(151-8)){
        *y+=charHeight;
        *x-=151;
    }
    //do nothing if y is off the screen, or if inchar is invalid
    if((*y>151)||(32>inChar>223)){
        return;
    }
    
    //set write window and counter
    Set_Write_Window(*x, *x+((charLen-1)*8), *y, (*y+charHeight));
    
    Epaper_Write_Command(CMD_WRITE_RAM);
    
    uint8_t i;
    uint8_t j;
    uint16_t readIndex=charOffset;	//index in font array of byte to read

    for(i=0;i<charHeight;i++){
	    for(j=0;j<charLen;j++){

        	uint8_t printChar;
        	if(colour==BLACK){
            		printChar=~(pgm_read_byte(&font[readIndex]));
        	}else{
            		printChar=pgm_read_byte(&font[readIndex]);
        	}

        	Epaper_Write_Data(printChar);

		readIndex++;
	   }
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
  
  while(1)
  { 
      
    Write_String(32, 64, BLACK, "This Is A Test");
    Epaper_Update();
    delay(500);
     
    //fill screen with grey
    Fill_Screen(GREY);  
    delay(500);   

    //draw some squares
    Draw_Rectangle(32,96,32,96,WHITE);   
    Draw_Rectangle(64,128,64,128,BLACK);
 
    Epaper_Update();
 
    Fill_Screen(BLACK);  
    delay(500);
    
    Fill_Screen(WHITE);
    delay(500);
  }
}

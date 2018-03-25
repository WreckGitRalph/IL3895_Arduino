/*
 * Port from STM32 to Arduino Uno of the EPAPER initialization example from GoodDisplay.
 * Created on March 11, 2018, 6:20 PM
 */

#include "screen.h"
#include <Arduino.h>
#include <SPI.h>

char vcom=0x52;	//-1.55V
extern u8 Epaper_border;
SPISettings devSPI(SPI_FREQ,MSBFIRST,SPI_MODE0);

#ifdef GDEM0154E97LT
//////////////////////////////////////////
//GDEM0154E97LT device-specific settings//
//////////////////////////////////////////

#define EPD_GATE_VOLTAGE    0x17	//VGH=20V
#define EPD_SRC_VOLTAGE_A   0x41	//VSH1=15V
#define EPD_SRC_VOLTAGE_B   0x0         //VSH2=0V
#define EPD_SRC_VOLTAGE_C   0x32        //VSL=-15V			
#define EPD_DUMMY_PERIOD    0x15  
#define EPD_GATE_PERIOD     0x0B

//default update waveform LUT for GDEM0154E97LT
const unsigned char LUT[]=
{						
0x80,	0xA5,	0x10,	0x0,	0x0,	0x0,	0x0,
0x10,	0xA5,	0x80,	0x0,	0x0,	0x0,	0x0,
0x80,	0xA5,	0x10,	0x0,	0x0,	0x0,	0x0,
0x10,	0xA5,	0x80,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x6,	0x8,	0x0,	0x0,	0x2,		
0xC,	0x0,	0xC,	0x0,	0x5,		
0x8,	0x6,	0x0,	0x0,	0x2,		
0x0,	0x0,	0x0,	0x0,	0x0,		
0x0,	0x0,	0x0,	0x0,	0x0,		
0x0,	0x0,	0x0,	0x0,	0x0,		
0x0,	0x0,	0x0,	0x0,	0x0
};

#endif

#ifdef BUFFER
//can't fit a whole frame in memory on most AVRs, maybe use [external?] EEPROM as swap space
#define NUM_SWAPS 8
#define BUFFER_SIZE (ALLSCREEN_BYTES/NUM_SWAPS)
#endif

/*
* Function name: EpaperIO_Init
 * Description : Initialize IO interface related to e-paper
 * Input : No
 * Output : None
 */	    
void EpaperIO_Init(void)
{
    
    //initialize Arduino SPI pins
    SPI.begin();
    //Initialize additional pins BUSY, DC, and RESET
    pinMode(Epaper_BUSY,INPUT);
    pinMode(Epaper_DC, OUTPUT);


}

/*
* Function name: SPI1_WriteByte
 * Description: SPIx reads and writes one byte
 * Input :TxData: bytes to be written
 * Output : Return value: bytes read
 */
void Epaper_Spi_WriteByte(u8 txData)
{				   			 
    digitalWrite(Epaper_CS,HIGH);
    SPI.beginTransaction(SPISettings(devSPI));
    digitalWrite(Epaper_CS,LOW);
    delayMicroseconds(5) ;
        
    SPI.transfer(txData);
    
    delayMicroseconds(5) ;
    digitalWrite(Epaper_CS,HIGH);
    SPI.endTransaction();
}

/*
* Function name: HalLcd_READBUSY
 * Description : Wait for e-paper busy signal to change from high to low
 * Input : No
 * Output : None
 */	
void Epaper_READBUSY(void)
{ 
  while(1)
  {	 
     if(digitalRead(Epaper_BUSY)==0) break;
		 delayMicroseconds(5) ;
  }  
	 delayMicroseconds(100) ;
}

/*
* Function name: Epaper_Write_Command
 * Description: Write command
 * Enter :cmd
 * Output : None
 */	
void Epaper_Write_Command(u8 cmd)
{
    
digitalWrite(Epaper_DC,LOW);  // D/C#   0:command  1:data
 Epaper_Spi_WriteByte(cmd);
 
}

/*
* Function name: Epaper_Write_Command
 * Description : Write data
 * Enter :cmd
 * Output : None
 */	
void Epaper_Write_Data(u8 data)
{
    
   digitalWrite(Epaper_DC,HIGH);  // D/C#   0:command  1:data
   Epaper_Spi_WriteByte(data);
   
}

/*
* Function name: Epaper_Update
 * Description : Electronic paper initialization program, sending commands and data to e-paper
 * Enter : None
 * Output : None
 */	
void Epaper_Update(void)
{
    //enable display update bypass option
    Epaper_Write_Command(CMD_UPDATE_CTRL_1);
    Epaper_Write_Data(0x40);    
    //enable clock signal, analog, LUT, pattern display, disable CP, OSC
    Epaper_Write_Command(CMD_UPDATE_CTRL_2);
    Epaper_Write_Data(0xC7); 
    //perform update
    Epaper_Write_Command(CMD_UPDATE);
    Epaper_READBUSY();	
}

/*
* Function name: Epaper_DeepSleep
 * Description : Electronic Paper Deep Sleep Mode
 * Enter : None
 * Output : None
 */	
void Epaper_DeepSleep(void)
{
   Epaper_Write_Command(CMD_SLEEP);
   Epaper_Write_Data(0x01);   
   delay(100); 	
   //Epaper_EN =0;///Epaper VCC Off
   delay(100); 	
}

/*
* Function name: Set_Counter
 * Description : Set display RAM counter. Coordinates start at zero.
 * Enter : x: x coordinate
 *          y: y coordinate
 * Output : None
 */	
void Set_Counter(uint16_t x, uint16_t y){
    //x is addressed in 19 bytes, so just use the closest byte
    //maybe do something later to skip the extra bits instead
    if(x>151){
        x=151;
    }
    if(y>151){
        y=151;
    }
    x=x/8;
    
    uint8_t y1=y&0xFF;
    uint8_t y2=(y>>8)&0xFF; 
    
    Epaper_Write_Command(CMD_X_CNT);     
    Epaper_Write_Data(x);	
    Epaper_Write_Command(CMD_Y_CNT);       
    Epaper_Write_Data(y1);
    Epaper_Write_Data(y2);
}

/*
* Function name: Set_Write_Window
 * Description : Define a window for writing display data. Coordinates start at zero.
 * Enter : x1: starting x coordinate
 *          x2: ending x coordinate
 *          y1: starting y coordinate
 *          y2: ending y coordinate
 * Output : None
 */	
void Set_Write_Window(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2){
    //x is addressed in 19 bytes, so just use the closest byte
    //maybe do something later to skip the extra bits instead
    uint8_t x_strt=x1/8;
    uint8_t x_end=x2/8;
    
    uint8_t y_strt1=y1&0xFF; 
    uint8_t y_strt2=(y1>>8)&0xFF;
    uint8_t y_end1=y2&0xFF;
    uint8_t y_end2=(y2>>8)&0xFF;
        
    Epaper_Write_Command(CMD_X_POS);  
     Epaper_Write_Data(x_strt);   
     Epaper_Write_Data(x_end);    
	
    Epaper_Write_Command(CMD_Y_POS);         
     Epaper_Write_Data(y_strt1);   
     Epaper_Write_Data(y_strt2);   
     Epaper_Write_Data(y_end1);   
     Epaper_Write_Data(y_end2);  
     Set_Counter(x1,y1);
     
}

/*
* Function name: Epaper_Init
 * Description : Electronic paper initialization program, sending commands and data to e-paper
 * Enter : None
 * Output : None
 */	

void Epaper_Init(void)
{  

//    delay(10);  
    Epaper_READBUSY();
    Epaper_Write_Command(CMD_RESET); 
    Epaper_READBUSY();
	
    //undocumented commands from the STM32 driver, just using the recommended settings
    Epaper_Write_Command(CMD_ANALOG_CTRL); //set analog block control       
     Epaper_Write_Data(0x54);
    Epaper_Write_Command(CMD_DIGITAL_CTRL); //set digital block control          
     Epaper_Write_Data(0x3B);
	
    Epaper_Write_Command(CMD_DRIVER_OUTPUT);       
     Epaper_Write_Data(0x97); //152 MUX gate lines
     Epaper_Write_Data(0x00); //152 MUX gate lines
     Epaper_Write_Data(0x00); //gate direction=0, scan mode=0, scan direction=0
     
    Epaper_Write_Command(CMD_TMP_CTRL);
     Epaper_Write_Data(0x80); //use internal temperature sensor
	
    Epaper_Write_Command(CMD_DATA_ENTRY_MODE);   
     Epaper_Write_Data(0x03);   //address counter auto-increments from top left to bottom right 

     //////////!!!!!!!!!!!/////////////////
     //////////!!!!!!!!!!//////////////////
    Set_Write_Window(0,151,0,151); //params here should be dynamic
     
    Epaper_Write_Command(CMD_BORDER_CTRL);
     Epaper_Write_Data(0x01);	//use GS transition LH for VBD
}

/*
* Function name: Epaper_LUT
 * Description : Loading waveforms for SSD1675
 * Enter :wave_data - device-specific driving waveform
 * Output : None
 */	
void Epaper_LUT(u8 * wave_data)
{        
  u8 count;
  int LUTSize=sizeof(LUT)/sizeof(LUT[0]);
  
  Epaper_Write_Command(CMD_WRITE_LUT);
  for(count=0;count<LUTSize;count++) Epaper_Write_Data(*wave_data++); 
  Epaper_READBUSY();
 
}

void LUT_Written_by_MCU(void)
{
	
   Epaper_Write_Command(CMD_WRITE_VCOM);    
    Epaper_Write_Data( vcom); //manually set VCOM to -1.55V, should change this to use VCOM sense
		
    Epaper_Write_Command(CMD_GATE_V);   
    Epaper_Write_Data(EPD_GATE_VOLTAGE);
		
    Epaper_Write_Command(CMD_SRC_V);       
    Epaper_Write_Data(EPD_SRC_VOLTAGE_A); 
    Epaper_Write_Data(EPD_SRC_VOLTAGE_B); 
    Epaper_Write_Data(EPD_SRC_VOLTAGE_C); 
		
    Epaper_Write_Command(CMD_DMY_LINE_T);      
    Epaper_Write_Data(EPD_DUMMY_PERIOD); 
    Epaper_Write_Command(CMD_GATE_LINE_T);        
    Epaper_Write_Data(EPD_GATE_PERIOD);
		
    Epaper_LUT((u8*)LUT); 
	
}


/*
* Function name: Epaper_Load_Image
 * Description : Set RAM to refresh picture
 * Input :datas
         :num
         : mode
 * Output : None
 */	
void Epaper_Load_Image(u8 *datas,u32 num,u8 mode)
{
  u32 i; 
  u8 tempOriginal;      
  u32 tempcol=0;
  u32 templine=0;

  //reset display counter
  Set_Counter(0,0);
	
    Epaper_READBUSY();
    
    if(mode==MONO)
        Epaper_Write_Command(CMD_WRITE_RAM);
    if(mode==RED)
	Epaper_Write_Command(0x26);   //write image for red displays?	
    
    for(i=0;i<num;i++){          
        tempOriginal=*(datas+templine*MAX_COLUMN_BYTES+tempcol);
        templine++;
        if(templine>=MAX_LINE_BYTES){
            tempcol++;
            templine=0;
        }     
     Epaper_Write_Data(tempOriginal);
    } 
    Epaper_Update();

}

/*
* Function Name: Fill_Screen
 * Description : fill screen with a solid colour
 * Input : colour: 0 for white, 1 for black, 2 for grey     <- put this in a define
 * Output : None
 */	
void Fill_Screen(u8 colour){
  u32 i,j;
  uint8_t fillData;
  
  switch(colour){
       //white
      case 0:
          fillData=0xFF;
          break;
      //black
      case 1:
          fillData=0x00;
          break;
     //grey
      case 2:
          fillData=0xAA;
  }

    //Set_Counter(0,0);
    Set_Write_Window(0,151,0,151);
	
    Epaper_READBUSY();
    
    Epaper_Write_Command(CMD_WRITE_RAM);   
    for(i=0;i<152;i++){
        for(j=0;j<19;j++){
            Epaper_Write_Data(fillData);
        }
    }
Epaper_Update();
	 
}
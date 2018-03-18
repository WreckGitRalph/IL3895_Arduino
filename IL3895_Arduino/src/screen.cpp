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

#ifdef DKE154
//////////////////////////////////////////
//GDEM0154E97LT device-specific settings//
//////////////////////////////////////////

#define EPD_GATE_VOLTAGE    0x17	//VGH=20V
#define EPD_SRC_VOLTAGE_A   0x41	//VSH1=15V
#define EPD_SRC_VOLTAGE_B   0x0         //VSH2=0V
#define EPD_SRC_VOLTAGE_C   0x32        //VSL=-15V			
#define EPD_DUMMY_PERIOD    0x15  
#define EPD_GATE_PERIOD     0x0B

//fast update waveform LUT for GDEM0154E97LT
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
//    pinMode(Epaper_RESET, OUTPUT);


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
  {	 //=1 BUSY
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
    delay(100); 	
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
* Function name: Epaper_Init
 * Description : Electronic paper initialization program, sending commands and data to e-paper
 * Enter : None
 * Output : None
 */	
void Epaper_Init(void)
{  

    delay(100); 
    //digitalWrite(Epaper_RESET,LOW);     
    delay(10); 
    //digitalWrite(Epaper_RESET,HIGH);  
    delay(10);  
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
	
    Epaper_Write_Command(CMD_DATA_ENTRY_MODE);   
     Epaper_Write_Data(0x11);   //address counter auto-increments from top left to bottom right 
		
    Epaper_Write_Command(CMD_X_POS);  
     Epaper_Write_Data(0x00);   //x starts at 0
    Epaper_Write_Data(0x12);    //x ends at (18+1)*8=152
	
    Epaper_Write_Command(CMD_Y_POS); //set Ram-Y address start/end position          
     Epaper_Write_Data(0x00);   //y starts 0x0097
    Epaper_Write_Data(0x97);    //y starts 0x0097
     Epaper_Write_Data(0x00);   //y ends at 0x0000
    Epaper_Write_Data(0x00);    //y ends at 0x0000
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
    Epaper_Write_Command(CMD_X_CNT);     
    Epaper_Write_Data(0x00);	
    Epaper_Write_Command(CMD_Y_CNT);       
    Epaper_Write_Data(0x97);
    Epaper_Write_Data(0x00);
	
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
* Function Name: Display_All_White
 * Description : refresh display full white
 * Input : No
 * Output : None
 */	
void Display_All_White(void)
{
  u32 i,j; 

  //reset display counter
    Epaper_Write_Command(CMD_X_CNT);     
    Epaper_Write_Data(0x00);	
    Epaper_Write_Command(CMD_Y_CNT);       
    Epaper_Write_Data(0x97);
    Epaper_Write_Data(0x00);

	
    Epaper_READBUSY();
    Epaper_Write_Command(CMD_WRITE_RAM);   
    for(i=0;i<152;i++){
        for(j=0;j<19;j++){
            Epaper_Write_Data(0xFF);
        }
    }
Epaper_Update();
	 
}

/*
* Function Name: Display_All_Black
 * Description : refresh display black
 * Input : No
 * Output : None
 */	
void Display_All_Black(void)
{
  u32 i,j; 

  //reset display counter
    Epaper_Write_Command(CMD_X_CNT);     
    Epaper_Write_Data(0x00);	
    Epaper_Write_Command(CMD_Y_CNT);       
    Epaper_Write_Data(0x97);
    Epaper_Write_Data(0x00);

	
    Epaper_READBUSY();
    Epaper_Write_Command(CMD_WRITE_RAM);   
    for(i=0;i<152;i++){
        for(j=0;j<19;j++){
            Epaper_Write_Data(0x00);
     }
   }
	 Epaper_Update();

}

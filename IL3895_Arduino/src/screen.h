/*
 * Port from STM32 to Arduino Uno of the EPAPER initialization example from GoodDisplay.
 * Created on March 11, 2018, 5:54 PM
 */


#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>

//some types used in the original driver
typedef unsigned char u8;
typedef unsigned long int u32;

#define __AVR_ATmega328P__
#define  GDEM0154E97LT 

#define  MONO 1
#define  RED  2
#define  WHITE 0xFF
#define  BLACK 0x00
#define  GREY 0xAA

/////////////////////////////
//display-specific settings//
/////////////////////////////
#ifdef GDEM0154E97LT
#define MAX_LINE_BYTES    19
#define MAX_COLUMN_BYTES	152
#define ALLSCREEN_BYTES   2888
#endif

#ifdef DKE213_1
#define MAX_LINE_BYTES    16
#define MAX_COLUMN_BYTES   250 
#define ALLSCREEN_BYTES   4000
#endif


#ifdef DKE213_2
#define MAX_LINE_BYTES    13
#define MAX_COLUMN_BYTES   212  
#define ALLSCREEN_BYTES   2756
#endif

#ifdef DKE290
#define MAX_LINE_BYTES     16
#define MAX_COLUMN_BYTES   296
#define ALLSCREEN_BYTES   4736
#endif

#ifdef DKE420
#define MAX_LINE_BYTES    50
#define MAX_COLUMN_BYTES  300
#define ALLSCREEN_BYTES   15000
#endif

/////////////////////////////////////
//microcontroller-specific settings//
/////////////////////////////////////
#ifdef __AVR_ATmega328P__
//SPI settings
#define SPI_FREQ    10000  //10 KHz, from STM32 sample. max is 20MHz

//pin definitions
#define  Epaper_BUSY   8       //high signifies display is busy
#define Epaper_DC     9       //data/command control pin
#define Epaper_CS     29      //SS
//#define Epaper_SCK    13    
//#define Epaper_SDO    11   //MOSI

#define  BUFFER
#endif

#ifdef __AVR_ATmega168P__
//SPI settings
#define SPI_FREQ    10000  //10 KHz, from STM32 sample. max is 20MHz

//pin definitions
#define  Epaper_BUSY   8       //high signifies display is busy
#define Epaper_DC     9       //data/command control pin
#define Epaper_CS     10      //SS
//#define Epaper_SCK    13    
//#define Epaper_SDO    11   //MOSI

#undef  BUFFER
#endif


/////////////////////////////////////
//SPI commands
/////////////////////////////////////
#define CMD_DRIVER_OUTPUT 0x01          //gate setting
#define CMD_GATE_V 0x03                 //set gate related driving voltage
#define CMD_SRC_V 0x04                  //set source output voltage magnitude
#define CMD_GATE_SCAN_POS 0x0F          //set the scanning start position of the gate driver
#define CMD_SLEEP 0x10                  //deep sleep mode control
#define CMD_DATA_ENTRY_MODE 0x11        //define data entry sequence
#define CMD_RESET 0x12                  //soft reset, sets commands and parameters to defaults
#define CMD_TMP_CTRL 0x1A               //write to temperature register
#define CMD_UPDATE 0x20                 //activate display update sequence
#define CMD_UPDATE_CTRL_1 0x21          //set update bypass options, used to display ram content
#define CMD_UPDATE_CTRL_2 0x22          //set update sequence options
#define CMD_WRITE_RAM 0x24              //write proceeding data to RAM
#define CMD_VCOM_SENSE 0x28             //read VCOM value
#define CMD_VCOM_SENSE_DRTN 0x29        //set duration for VCOM sensing
#define CMD_VCOM_OTP 0x2A               //program VCOM register into OTP (non-volatile memory)
#define CMD_WRITE_VCOM 0x2C             //write VCOM register from MCU interface
#define CMD_READ_STTS 0x2F              //read status bit
#define CMD_WS_OTP 0x30                 //program waveform setting into OTP
#define CMD_WRITE_LUT 0x32              //write LUT register
#define CMD_SET_OTP 0x36                //program OTP selection
#define CMD_OTP_CTRL 0x37               //parameters for OTP selection
#define CMD_DMY_LINE_T 0x3A             //set dummy line period in terms of TGate
#define CMD_GATE_LINE_T 0x3B            //set gate line period TGate in us
#define CMD_BORDER_CTRL 0x3C            //select border waveform for VBD
#define CMD_X_POS 0x44                  //specify the x start/end positions of the window address
#define CMD_Y_POS 0x45                  //specify the y start/end positions of the window address
#define CMD_X_CNT 0x4E                  //set the RAM x counter value
#define CMD_Y_CNT 0x4F                  //set the RAM y counter value
#define CMD_ANALOG_CTRL 0x74            //set analog block control
#define CMD_DIGITAL_CTRL 0x7E           //set digital block control

void EpaperIO_Init(void);
void Epaper_READBUSY(void);
void Epaper_Write_Command(u8 cmd);
void Epaper_Write_Data(u8 data);
void Set_Write_Window(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
void Epaper_Init(void);
void LUT_Written_by_MCU(void);
void Epaper_Load_Image(u8 *datas,u32 num,u8 mode);
void Epaper_Update(void);
void Epaper_DeepSleep(void);
void Set_Counter(uint16_t x, uint16_t y);
void Fill_Screen(u8 colour);

#endif /* SCREEN_H */


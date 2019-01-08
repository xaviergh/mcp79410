/*! 
* @file mcp79410.c
*
* @brief Simple driver for the Microchip MCP79410 RTC.
*
* @author Xavier Garcia Herrera.
*
* @par
* Version 1.0 2015
*/
#include "mcp79410.h"

void mcp79410_i2c_init(void)
{
  #use i2c(master, sda=I2C_SDA, scl=I2C_SCL, FORCE_HW)
}

void mcp79410_init(mcp79410_mode_t mfp_mode)
{

  mcp79410_i2c_init();

  uint8_t seconds = 0;

  i2c_start(); 
  i2c_write(RTC_WRITE_CONTROL);     // WR to RTC 
  i2c_write(0x00);                  // REG 0 
  i2c_start(); 
  i2c_write(RTC_READ_CONTROL);      // RD from RTC 
  seconds = bcd2int(i2c_read(0));   // Read current "seconds" in MCP79410
  i2c_stop(); 
  seconds &= 0x7F; 

  delay_us(3);

  i2c_start(); 
  i2c_write(RTC_WRITE_CONTROL);      // WR to RTC 
  i2c_write(0x00);      // REG 0 
  i2c_write(int2bcd(seconds) | 0x80);     // Start oscillator with current "seconds value 
  i2c_stop();
  
  delay_us(3);

  i2c_start(); 
  i2c_write(RTC_WRITE_CONTROL);      // WR to RTC 
  i2c_write(0x07);      // Control Register

  switch(mfp_mode)
  {
    case MCP79410_MFP_SQW_1Hz:
      i2c_write(0x40);
    break;

    case MCP79410_MFP_SQW_4096kHz:
      i2c_write(0x41);
    break;

    case MCP79410_MFP_SQW_8192kHz:
      i2c_write(0x42);
    break;

    case MCP79410_MFP_SQW_32768kHz:
      i2c_write(0x43);
    break;

    case MCP79410_MFP_DISABLED:
      i2c_write(0x00);
    break;
  }

  i2c_stop();
} 

void 
mcp79410_set_date_time (uint8_t day, uint8_t mth, uint8_t year, uint8_t dow, 
                        uint8_t hr, uint8_t min, uint8_t sec)
{ 
  sec &= 0x7F; 
  hr &= 0x3F; 

  //TODO: check leap year and configure reg 5

  i2c_start(); 
  i2c_write(RTC_WRITE_CONTROL);     // I2C write address 
  i2c_write(0x00);                  // Start at REG 0 - Seconds 
  i2c_write(int2bcd(sec) | 0x80);   // REG 0 
  i2c_write(int2bcd(min));          // REG 1 
  i2c_write(int2bcd(hr));           // REG 2 
  i2c_write(int2bcd(dow) | 0x38);   // REG 3 
  i2c_write(int2bcd(day));          // REG 4 
  i2c_write(int2bcd(mth));          // REG 5 
  i2c_write(int2bcd(year));         // REG 6 
  //i2c_write(0x43);                // REG 7 - 32.768 KHz Square Output
  i2c_write(0x40);
  i2c_stop(); 
} 

void 
mcp79410_get_date (rtc_t* mcp_rtc)
{ 
  
  i2c_start(); 
  i2c_write(RTC_WRITE_CONTROL); 
  i2c_write(0x03);            // Start at REG 3 - Day of week 
  i2c_start(); 
  i2c_write(RTC_READ_CONTROL);
  mcp_rtc->dow    = bcd2int(i2c_read() & 0x7f);   // REG 3 
  mcp_rtc->day    = bcd2int(i2c_read() & 0x3f);   // REG 4 
  mcp_rtc->month  = bcd2int(i2c_read() & 0x1f);   // REG 5 
  mcp_rtc->year   = bcd2int(i2c_read(0));         // REG 6 
  i2c_stop();
} 

void 
mcp79410_get_time (rtc_t* mcp_rtc)
{ 
  
  i2c_start(); 
  i2c_write(RTC_WRITE_CONTROL); 
  i2c_write(0x00);            // Start at REG 0 - Seconds 
  i2c_start(); 
  i2c_write(RTC_READ_CONTROL); 
  mcp_rtc->sec    = bcd2int(i2c_read() & 0x7f); 
  mcp_rtc->min    = bcd2int(i2c_read() & 0x7f); 
  mcp_rtc->hour  = bcd2int(i2c_read(0) & 0x3f); 
  i2c_stop();  
} 

uint8_t int2bcd(uint8_t int_value)
{
  return ((int_value / 10) << 4) | (int_value % 10);
}

uint8_t bcd2int(uint8_t bcd_value)
{
  return (bcd_value >> 4)*10 + (bcd_value & 0x0F);
} 

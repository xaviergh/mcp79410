/*! 
* @file mcp79410.h
*
* @brief Simple driver for the Microchip MCP79410 RTC.
*
* @author Xavier Garcia Herrera.
*
* @par
* Xavier Garcia Herrera
* Version 1.1 2016
*/

#ifndef _MCP79410_H
#define _MCP79410_H

#include <stdint.h> //generic typedefs
#include <stdbool.h> //bool typedef

/*!
  MCP79410 Multifunction Pin mode
*/
typedef enum
{
  MCP79410_MFP_SQW_1Hz,       /*!< 1 Hz Square Wave output */
  MCP79410_MFP_SQW_4096kHz,   /*!< 4.096 kHz Square Wave output */
  MCP79410_MFP_SQW_8192kHz,   /*!< 8.192 kHz Square Wave output */
  MCP79410_MFP_SQW_32768kHz,  /*!< 32.768 kHz Square Wave output */
  MCP79410_MFP_DISABLED,      /*!< Multifunction pin disabled */
  MCP79410_MFP_ALARM,         /*!< Multifunction pin alarm output */
} mcp79410_mode_t;

/*!
  RTC Struct
*/
typedef struct rtc
{
  uint8_t   sec;
  uint8_t   min;
  uint8_t   hour;
  uint8_t   day;
  uint8_t   month;
  uint8_t   year;
  uint16_t  dow;
}rtc_t;

/*! @name MCP79410 Definitions
** @{
*/
/*!
  Commands
*/
#define RTC_SLAVE_ADDR    0b11011110  /*!< RTC Slave Address */
#define RTC_WRITE_CONTROL 0xDE        /*!< RTC Write Mask */
#define RTC_READ_CONTROL  0xDF        /*!< RTC Read Mask */
/*!
  Pin definitions
*/
#ifndef I2C_SDA
#define I2C_SDA PIN_C4
#endif

#ifndef I2C_SCL
#define I2C_SCL PIN_C3
#endif
/*!
  Registers: TODO
*/
/** @} */

/* Function prototypes*/
void 
mcp79410_i2c_init(void);

void 
mcp79410_init (mcp79410_mode_t mfp_mode);

void 
mcp79410_set_date_time (uint8_t day, uint8_t mth, uint8_t year, uint8_t dow, 
                        uint8_t hr, uint8_t min, uint8_t sec);

void 
mcp79410_get_date (rtc_t* mcp_rtc);

void 
mcp79410_get_time (rtc_t* mcp_rtc);

uint8_t 
int2bcd(uint8_t int_value);

uint8_t 
bcd2int(uint8_t bcd_value); 


#endif /* _MCP79410_H */

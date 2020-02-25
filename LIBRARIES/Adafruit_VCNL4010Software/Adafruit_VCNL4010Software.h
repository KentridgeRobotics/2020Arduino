/**************************************************************************/
/*!
  @file     Adafruit_VCNL4010Software.h

  Author: K. Townsend (Adafruit Industries)
	License: BSD (see license.txt)

	This is a library for the Aadafruit VCNL4010 proximity sensor breakout board
	----> http://www.adafruit.com/products/466

	Adafruit invests time and resources providing this open source code,
	please support Adafruit and open-source hardware by purchasing
	products from Adafruit!

  v1.0  - First release
*/
/**************************************************************************/

#include "Arduino.h"

#include <SoftwareWire.h>

#define VCNL4010_I2CADDR_DEFAULT 0x13		///< I2C address of the sensor

/** Registers */
#define VCNL4010_COMMAND 0x80           ///< Command
#define VCNL4010_PRODUCTID 0x81         ///< Product ID Revision
#define VCNL4010_PROXRATE 0x82          ///< Proximity rate
#define VCNL4010_IRLED 0x83             ///< IR LED current
#define VCNL4010_AMBIENTPARAMETER 0x84  ///< Ambient light parameter
#define VCNL4010_AMBIENTDATA 0x85       ///< Ambient light result (16 bits)
#define VCNL4010_PROXIMITYDATA 0x87     ///< Proximity result (16 bits)
#define VCNL4010_INTCONTROL 0x89        ///< Interrupt control
#define VCNL4010_LOWTHRESHOLD 0x8A      ///< Low threshold value (16 bits)
#define VCNL4010_HITHRESHOLD 0x8C       ///< High threshold value (16 bits)
#define VCNL4010_INTSTAT 0x8E           ///< Interrupt status
#define VCNL4010_MODTIMING 0x8F         ///< Proximity modulator timing adjustment

/** Proximity measurement rate */
typedef enum
  {
    VCNL4010SOFTWARE_1_95    = 0, // 1.95     measurements/sec (Default)
    VCNL4010SOFTWARE_3_90625 = 1, // 3.90625  measurements/sec
    VCNL4010SOFTWARE_7_8125  = 2, // 7.8125   measurements/sec
    VCNL4010SOFTWARE_16_625  = 3, // 16.625   measurements/sec
    VCNL4010SOFTWARE_31_25   = 4, // 31.25    measurements/sec
    VCNL4010SOFTWARE_62_5    = 5, // 62.5     measurements/sec
    VCNL4010SOFTWARE_125     = 6, // 125      measurements/sec
    VCNL4010SOFTWARE_250     = 7, // 250      measurements/sec
  } vcnl4010software_freq;

/** Values for command register */
#define VCNL4010_MEASUREPROXIMITY 0x08  ///< Start a single on-demand proximity measurement
#define VCNL4010_MEASUREAMBIENT   0x10  ///< Start a single on-demand ambient light measurement
#define VCNL4010_PROXIMITYREADY   0x20  ///< Read-only - Value = 1 when proximity measurement data is available
#define VCNL4010_AMBIENTREADY     0x40  ///< Read-only - Value = 1 when ambient light measurement data is available

/**************************************************************************/
/*!
    @brief  The VCNL4010 class
*/
/**************************************************************************/
class Adafruit_VCNL4010Software {
  public:
    Adafruit_VCNL4010Software();
    boolean begin(uint8_t sdaPin, uint8_t sclPin, uint8_t a = VCNL4010_I2CADDR_DEFAULT);

    uint8_t getLEDcurrent(void);
    void setLEDcurrent(uint8_t c);

    void setFrequency(vcnl4010software_freq f);
    uint16_t readProximity(void);
    uint16_t readAmbient(void);

  private:
    void write8(uint8_t address, uint8_t data);
    uint16_t read16(uint8_t address);
    uint8_t read8(uint8_t address);

    uint8_t _i2caddr;
    SoftwareWire _wire;
};

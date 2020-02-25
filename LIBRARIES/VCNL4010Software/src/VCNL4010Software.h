/*! @file VCNL4010Software.h

 @mainpage Arduino Library Header for the VCNL4010Software Proximity detector

 @section VCNL4010Software_intro_section Description

Class definition header for the VCNL4010Software chip from Vishay Semiconductors. This is a Fully Integrated Proximit and
and Ambient Light Sensor with Infrared Emitter, I2C Interface and Interrupt Function. The data sheet for the 
VCNL4010Software can be found at http://www.vishay.com/docs/83462/vcnl4010.pdf and there is a breakout board from Adafruit
with additional information located at https://www.adafruit.com/product/466. \n\n
The most recent version of the library can be downloaded from https://github.com/SV-Zanshin/VCNL4010Software/archive/master.zip 
and a detailed description of the library and it's functions is at the github wiki located 
in https://github.com/SV-Zanshin/VCNL4010Software/wiki \n\n
When the class is instantiated the default mode is to have triggered readings for the ambient light sensor as well as 
for the proximity sensor.  In order to speed up processing the class will trigger the two types of measurements 
preemptively, meaning that as soon that either of the results have been read the next measurement is started 
automatically.  If the continuous mode is activated for either of the ambient or proximity sensors then no specific 
measurements are triggered since the device takes care of that internally.

@section VCNL4010Software_license GNU General Public License v3.0
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General
Public License as published by the Free Software Foundation, either version 3 of the License, or (at your
option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details. You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

@section VCNL4010Software_author Author

Written by Arnd\@SV-Zanshin

@section VCNL4010Software_versions Changelog

Version| Date       | Developer                     | Comments
------ | ---------- | ----------------------------- | --------------------------------------------------------------
1.0.8  | 2019-01-24 | https://github.com/SV-Zanshin | Issue #9 - Doxygen Documentation and Travis-CI changes
1.0.8  | 2018-07-22 | https://github.com/SV-Zanshin | Corrected I2C Datatypes
1.0.8  | 2018-07-02 | https://github.com/SV-Zanshin | Added guard code against multiple I2C constant definitions
1.0.8  | 2018-06-29 | https://github.com/SV-Zanshin | https://github.com/SV-Zanshin/VCNL4010Software/issues/8 I2C speed slct
1.0.5  | 2017-09-02 | https://github.com/SV-Zanshin | Added option to begin() to allow for different I2C Address
1.0.5c | 2017-09-01 | https://github.com/SV-Zanshin | Re-introduced 170?s delay in I2C read statements after hangs
1.0.5b | 2017-08-31 | https://github.com/SV-Zanshin | Removed 170?s delay in I2C read statements
1.0.5a | 2017-08-31 | https://github.com/Koepel     | Bug https://github.com/SV-Zanshin/VCNL4010Software/issues/4
1.0.4  | 2017-08-29 | https://github.com/SV-Zanshin | Changed method on I2C reads to avoid hang w/out extra call
1.0.3a | 2017-08-29 | https://github.com/Danny24    | Corrected potential I2C hang condition when no device present
1.0.2  | 2017-07-31 | https://github.com/SV-Zanshin | Removed default definitions from function definitions, kept them 
                                                      in the prototype definitions as this caused compiler errors on 
                                                      non-Windows based machines
1.0.1  | 2017-01-02 | https://github.com/SV-Zanshin | Moved readByte function back into private area
1.0.0  | 2017-01-01 | https://github.com/SV-Zanshin | Fixed error on continuous mode with proximity
1.0.b2 | 2016-12-31 | https://github.com/SV-Zanshin | Continued coding
1.0.b1 | 2016-12-30 | https://github.com/SV-Zanshin | Created class
*/
#include "Arduino.h"  // Arduino data type definitions
#include <SoftwareWire.h>     // Standard I2C "Wire" library
#ifndef VCNL4010Software_h    // Guard code definition
  /*! @brief Guard code definition for the VCNL4010Software Library */
  #define VCNL4010Software_h  // Define the name inside guard code
/********************************************
** Declare all constants used in the class **
********************************************/
#ifndef I2C_MODES // I2C related constants
  /*! @brief  Guard code definition for the various I2C modes */
  #define I2C_MODES // Guard code to prevent multiple declarations
  const uint32_t I2C_STANDARD_MODE               =  100000; ///< Default normal I2C 100KHz speed
  const uint32_t I2C_FAST_MODE                   =  400000; ///< Fast mode
  const uint32_t I2C_FAST_MODE_PLUS_MODE         = 1000000; ///< Really fast mode
  const uint32_t I2C_HIGH_SPEED_MODE             = 3400000; ///< Turbo mode
#endif
  const uint8_t  VCNL4010Software_ADDRESS                =    0x13; ///< Device address, fixed value
  const uint8_t  VCNL4010Software_COMMAND_REG            =    0x80; ///< Register containing commands
  const uint8_t  VCNL4010Software_PRODUCT_REG            =    0x81; ///< Register containing product ID
  const uint8_t  VCNL4010Software_PROXIMITY_RATE_REG     =    0x82; ///< Register containing sampling rate
  const uint8_t  VCNL4010Software_LED_CURRENT_REG        =    0x83; ///< Register containing IR LED mA
  const uint8_t  VCNL4010Software_AMBIENT_PARAMETER_REG  =    0x84; ///< Register containing ambient set
  const uint8_t  VCNL4010Software_AMBIENT_LIGHT_REG      =    0x85; ///< Register containing ambient data
  const uint8_t  VCNL4010Software_PROXIMITY_REG          =    0x87; ///< Register containing Proximity
  const uint8_t  VCNL4010Software_INTERRUPT_REG          =    0x89; ///< Register containing Interrupts
  const uint8_t  VCNL4010Software_LOW_THRESHOLD_MSB_REG  =    0x8A; ///< MSB of low threshold value
  const uint8_t  VCNL4010Software_LOW_THRESHOLD_LSB_REG  =    0x8B; ///< LSB of low threshold value
  const uint8_t  VCNL4010Software_HIGH_THRESHOLD_MSB_REG =    0x8C; ///< MSB of high threshold value
  const uint8_t  VCNL4010Software_HIGH_THRESHOLD_LSB_REG =    0x8D; ///< LSB of high threshold value
  const uint8_t  VCNL4010Software_INTERRUPT_STATUS_REG   =    0x8E; ///< Interrupt status register
  const uint8_t  VCNL4010Software_PROXIMITY_TIMING_REG   =    0x8F; ///< Register containing ProxTiming
  const uint8_t  VCNL4010Software_PRODUCT_VERSION        =    0x21; ///< Current product ID
  const uint8_t  VCNL4010Software_I2C_DELAY_MICROSECONDS =     200; ///< I2C Delay in communications
  /*!
  * @class VCNL4010Software
  * @brief Main class definition
  */
  class VCNL4010Software 
  {
    public:
      VCNL4010Software();
      ~VCNL4010Software();
      bool     begin(void);                                                   // Overloaded just device address   //
      bool     begin(const uint8_t  deviceAddress);                           // Overloaded just device address   //
      bool     begin(const uint32_t i2CSpeed);                                // Overloaded just speed            //
      bool     begin(const uint8_t  deviceAddress,                            // Start I2C communications         //
                     const uint32_t i2CSpeed);                                // specifying both parameters       //
      void     setProximityHz(const uint8_t Hz=2);                            // Set proximity Hz sampling rate   //
      void     setLEDmA(const uint8_t mA=20);                                 // Set milliamperes used by IR LED  //
      void     setProximityFreq(const uint8_t value=0);                       // Set Frequency value from list    //
      void     setAmbientLight(const uint8_t sample=2, const uint8_t avg=32); // Set samples and averaging        //
      void     setAmbientContinuous(const bool ContinuousMode=true);          // Cont. Ambient sampling on/off    //
      void     setProximityContinuous(const bool ContinuousMode=true);        // Cont. Proximity sampling on/off  //
      void     setInterrupt(const uint8_t  count         = 1,                 //                                  //
                            const bool     ProxReady     = false,             //                                  //
                            const bool     ALSReady      = false,             //                                  //
                            const bool     ProxThreshold = false,             //                                  //
                            const bool     ALSThreshold  = false,             //                                  //
                            const uint16_t lowThreshold  = 0,                 //                                  //
                            const uint16_t highThreshold = UINT16_MAX );      //                                  //
      uint16_t getAmbientLight();                                             // Retrieve ambient light reading   //
      uint16_t getProximity();                                                // Retrieve proximity reading       //
      uint8_t  getInterrupt();                                                // Retrieve Interrupt bits          //
      void     clearInterrupt(const uint8_t intVal=0);                        // Clear Interrupt bits             //
    private:
      uint8_t  readByte(const uint8_t addr);
      uint16_t readWord(const uint8_t addr);
      void     writeByte(const uint8_t addr, const uint8_t data);
      uint8_t _TransmissionStatus;                     // I2C transmission status
      bool    _ContinuousAmbient   = false;            // If mode turned on for Ambient readings
      bool    _ContinuousProximity = false;            // If mode turned on for Proximity readings
      uint8_t _I2Caddress          = VCNL4010Software_ADDRESS; // Default to standard I2C address
  }; // of VCNL4010Software class definition
#endif

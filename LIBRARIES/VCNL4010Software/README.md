# VCNL4010 library [![Build Status](https://travis-ci.org/SV-Zanshin/VCNL4010.svg?branch=master)](https://travis-ci.org/SV-Zanshin/VCNL4010) [![DOI](https://www.zenodo.org/badge/77754132.svg)](https://www.zenodo.org/badge/latestdoi/77754132)
<img src="https://github.com/SV-Zanshin/VCNL4010/blob/master/Images//VCNL4010_square.jpg" width="175" align="right"/> *Arduino* library for using the Vishay VCNL4010 sensor. The sensor is a fully integrated proximity and ambient light sensor with infrared emitter, I2C interface and interrupt function in a small package. It is available as a breakout board from Adafruit at https://www.adafruit.com/product/466.  The VCNL4010 datasheet can be found at  http://www.vishay.com/docs/83462/vcnl4010.pdf.

## Ambient light sensing
The ambient light sensor (ALS) returns a 16-bit value starting at close to 0 for no light and goes upwards as the light level increases.

## Proximity sensing
The proximity sensor has a range of approximately 200mm (2 cm, or 3/4 of an inch) and goes from 65536 when the closest reflector is touching the device to a value of around 2000 when nothing is being reflected in range.

A detailed library description and further details are available at the [GitHub VCNL4010 Wiki](https://github.com/SV-Zanshin/VCNL4010/wiki)

![Zanshin Logo](https://www.sv-zanshin.com/r/images/site/gif/zanshinkanjitiny.gif) <img src="https://www.sv-zanshin.com/r/images/site/gif/zanshintext.gif" width="75"/>

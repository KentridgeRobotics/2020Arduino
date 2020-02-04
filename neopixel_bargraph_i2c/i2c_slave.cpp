#include <arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "i2c_slave.h"
#include "defines.h"

extern Adafruit_NeoPixel strip;

I2CSlave::Registers I2CSlave::_reg{};
uint8_t I2CSlave::_offset = 0;
uint8_t I2CSlave::_length = 0;
void (*I2CSlave::user_onReceive)(void);

I2CSlave::I2CSlave(int sda, int scl, uint8_t addr)
    : _sda(sda), _scl(scl), _addr(addr) {}

bool I2CSlave::begin() {
  Wire.begin(_sda, _scl, _addr);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

enum ReceiveState {
  RECEIVE_STATE_OFFSET,
  RECEIVE_STATE_LENGTH,
  RECEIVE_STATE_DATA,
};

void I2CSlave::receiveEvent(size_t length) {
  ReceiveState state = RECEIVE_STATE_OFFSET;
  while (Wire.available() > 0) {
    switch (state) {
    case RECEIVE_STATE_OFFSET:
      _offset = Wire.read();

      // If you are trying to read past the end of the data, then read from 0
      if (_offset > sizeof(_reg)) {
        _offset = 0;
      }
      state = RECEIVE_STATE_LENGTH;
      break;
    case RECEIVE_STATE_LENGTH:
      _length = Wire.read();

      // truncate length so the most you can read is from the offset to the end.
      if (_length > sizeof(_reg) - _offset) {
        _length = sizeof(_reg) - _offset;
      }
      state = RECEIVE_STATE_DATA;
      break;
    case RECEIVE_STATE_DATA:
      // In the case where we are writing registers, then wrap around
      _reg.raw[_offset] = Wire.read();
      _offset += 1;
      _offset %= sizeof(_reg);
      break;
    }
  }

  if (user_onReceive) {
    user_onReceive();
  }
}

void I2CSlave::requestEvent() {
  while (_length > 0) {
    Wire.write(_reg.raw[_offset]);
    _length--;
  }
}

I2CSlave::Registers I2CSlave::getRegisters() {
  Registers reg;
  noInterrupts();
  reg = _reg;
  interrupts();
  return _reg;
}

void I2CSlave::setRegisters(const Registers &reg) {
  noInterrupts();
  _reg = reg;
  interrupts();
}

void I2CSlave::onReceive(void (*function)(void)) { user_onReceive = function; }

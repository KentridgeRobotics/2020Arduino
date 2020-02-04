#pragma once

class I2CSlave {
public:
  I2CSlave(int sda, int scl, uint8_t addr);
  bool begin();
  void onReceive(void (*)(void));

  union Registers {
    uint8_t raw[4]; // the whole size of the simulated register bank
    struct bargraph_t {
      uint8_t red;
      uint8_t green;
      uint8_t blue;
      uint8_t count;
    } bargraph;
    Registers() : raw(){};
  };

  Registers getRegisters();
  void setRegisters(const Registers &reg);

private:
  static void receiveEvent(size_t length);
  static void requestEvent();
  static void (*user_onReceive)(void);
  int _sda;
  int _scl;
  uint8_t _addr;
  static Registers _reg;
  static uint8_t _offset;
  static uint8_t _length;
};

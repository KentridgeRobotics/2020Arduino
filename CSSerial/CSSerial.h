#pragma once
#include <Arduino.h>

/*
  Run during Arduino setup to initialize and open checksumed serial port. A
  2-byte header, formatted as a short, is required as identification for the
  Arduino to differentiate it from others. An optional baud rate can be
  supplied.
*/
void serialBegin(short header, long baudRate = 115200);

/*
  Run during Arduino loop to check for available checksumed serial messages.
  Returns the number of bytes read from the message, between 0 and 256. Returns
  -1 if no message was available.
*/
int32_t serialAvailable();

/*
  Run after all data has been added to the buffer and is ready to be sent to the
  master. Running will send the data to the master. Ensure this is run prior to
  checking for new available serial messages to ensure that data is not
  overwritten.
*/
void sendData();

/*
  Run after a message has been received to get a pointer to the data buffer.
  Data received can be extracted here for processing prior to replying. Adding
  data to the buffer for sending will overwrite the buffer contents.
*/
byte* getDataBuffer();
byte* getChecksumArray();
byte* getMsgLengthArray();
uint16_t* getChecksum();
uint16_t* getMsgLength();

/*
  Run after a message has been received to add the given data to the buffer for
  sending back to the master. Each time this is run in succession, the data will
  be added to the buffer in order, overwriting anything received from the
  master.
*/
template<typename T>
void addData(T input) {
	byte* inputPtr = (byte*) &input;
	for (int i = 0; i < sizeof(input); i++) {
		getDataBuffer()[(*getMsgLength()) + i] = *(inputPtr + i);
		(*getChecksum()) += *(inputPtr + i);
	}
	(*getMsgLength()) += sizeof(input);
}
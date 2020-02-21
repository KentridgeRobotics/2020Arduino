#include "CSSerialProtocol.h"
#include <Arduino.h>

#define SERIAL_TIMEOUT 5

class CSSerialProtocol {
public:
	union shortArray {
		byte array[2];
		uint16_t num;
	} checksum;
	union shortArray msgLength;
};

CSSerialProtocol csSerialProtocol;

boolean foundSync, hasSync, hasHeader;
uint16_t recCount, currentChecksum, tempLen;
byte sync[2] = {0x55, 0x55};
byte poll[2] = {0x81, 0x42};
byte response[2];
byte dataBuffer[256];

void initSerial(short header, long baudRate = 115200) {
	response[0] = (byte)header;
	response[1] = (byte)(header >> 8);
	Serial.begin(baudRate);
}

void reset() {
	recCount = 0;
	currentChecksum = 0;
	foundSync = false;
	hasSync = false;
	hasHeader = false;
	csSerialProtocol.checksum.num = 0;
	csSerialProtocol.msgLength.num = 0;
}

void sendData() {
	Serial.write(sync, 2);
	Serial.write(response, 2);
	Serial.write(csSerialProtocol.checksum.array, 2);
	Serial.write(csSerialProtocol.msgLength.array, 2);
	Serial.write(dataBuffer, csSerialProtocol.msgLength.num);
}

byte* getDataBuffer() {
	return dataBuffer;
}

byte* getChecksumArray() {
	return csSerialProtocol.checksum.array;
}

byte* getMsgLengthArray() {
	return csSerialProtocol.msgLength.array;
}

uint16_t* getChecksum() {
	return &csSerialProtocol.checksum.num;
}

uint16_t* getMsgLength() {
	return &csSerialProtocol.msgLength.num;
}

unsigned long lastRec, startRec;

int32_t runSerial() {
	if (Serial.available() > 0) {
		byte in = Serial.read();
		if (!foundSync) {
			if (in == 0x55) {
				foundSync = true;
			}
		} else if (!hasSync) {
			if (in != 0x55) {
				hasSync = true;
				recCount = 0;
				startRec = millis();
			}
		}
		if (hasSync) {
			if (!hasHeader) {
				if (recCount < 2) {
					if (in != poll[recCount]) {
						reset();
						return -1;
					}
				} else if (recCount < 4) {
					csSerialProtocol.checksum.array[recCount % 2] = in;
				} else if (recCount < 6) {
					csSerialProtocol.msgLength.array[recCount % 2] = in;
					if (recCount == 5) {
						hasHeader = true;
						recCount = 0;
						if (recCount >= csSerialProtocol.msgLength.num) {
							reset();
							return 0;
						}
					}
				}
			} else {
				dataBuffer[recCount] = in;
				currentChecksum += in;
				if (recCount >= csSerialProtocol.msgLength.num) {
					if (currentChecksum == csSerialProtocol.checksum.num) {
						tempLen = csSerialProtocol.msgLength.num;
						reset();
						return tempLen;
					} else {
						reset();
					}
				}
			}
			recCount++;
		}
	} else {
		if (hasSync && millis() - startRec > SERIAL_TIMEOUT) {
			reset();
		}
	}
	return -1;
}
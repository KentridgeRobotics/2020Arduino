#include "CSSerial.h"
#include <Arduino.h>

#define SERIAL_TIMEOUT 5
#define VALID_HEADER_ARR_SIZE 16

#define NO_SYNC 0
#define SYNC_HALF 1
#define SYNC 2
#define HEADER_HALF 3
#define HEADER 4
#define CHECKSUM_HALF 5
#define CHECKSUM 6
#define LENGTH_HALF 7
#define RECEIVING 8
#define DONE 9

class CSSerialProtocol {
public:
	union shortArray {
		byte array[2];
		uint16_t num;
	} checksum;
	union shortArray msgLength;
};

CSSerialProtocol csSerialProtocol;

unsigned long startRec;

uint16_t bytesRead, currentChecksum, currentHeader;
uint8_t state, i;

byte sync[2] = {0x55, 0x55};
uint16_t validHeaders[VALID_HEADER_ARR_SIZE] = {0x4281};
uint8_t registeredHeaders = 1;
byte response[2];
byte dataBuffer[256];

void serialBegin(uint16_t header, long baudRate = 115200) {
	response[0] = (byte)header;
	response[1] = (byte)(header >> 8);
	Serial.begin(baudRate);
}

void resetRead() {
	currentChecksum = 0;
	csSerialProtocol.checksum.num = 0;
	csSerialProtocol.msgLength.num = 0;
	state = NO_SYNC;
}

void sendData() {
	Serial.write(sync, 2);
	Serial.write(response, 2);
	Serial.write(csSerialProtocol.checksum.array, 2);
	Serial.write(csSerialProtocol.msgLength.array, 2);
	Serial.write(dataBuffer, csSerialProtocol.msgLength.num);
	resetRead();
}

void registerHeader(uint16_t header) {
	if (registeredHeaders < 16) {
		validHeaders[registeredHeaders] = header;
		registeredHeaders++;
	}
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

uint16_t getHeader() {
	return currentHeader;
}

bool processByte(byte in) {
	switch(state) {
	case NO_SYNC:
		if (in == sync[0])
			state = SYNC_HALF;
		return true;
	case SYNC_HALF:
		if (in == sync[1]) {
			state = SYNC;
			startRec = millis();
			return true;
		} else
			resetRead();
		return false;
	case SYNC:
		currentHeader = in;
		state = HEADER_HALF;
		return true;
	case HEADER_HALF:
		currentHeader = currentHeader | (in << 8);
		for (i = 0; i < registeredHeaders; i++) {
			if (validHeaders[i] == currentHeader) {
				state = HEADER;
				return true;
			}
		}
		resetRead();
		return false;
	case HEADER:
		csSerialProtocol.checksum.array[0] = in;
		state = CHECKSUM_HALF;
		return true;
	case CHECKSUM_HALF:
		csSerialProtocol.checksum.array[1] = in;
		state = CHECKSUM;
		return true;
	case CHECKSUM:
		csSerialProtocol.msgLength.array[0] = in;
		state = LENGTH_HALF;
		return true;
	case LENGTH_HALF:
		csSerialProtocol.msgLength.array[1] = in;
		bytesRead = 0;
		if (csSerialProtocol.msgLength.num == 0)
			state = DONE;
		return true;
	case RECEIVING:
		dataBuffer[bytesRead] = in;
		currentChecksum += in;
		if (bytesRead >= csSerialProtocol.msgLength.num) {
			if (currentChecksum == csSerialProtocol.checksum.num) {
				state = DONE;
				return true;
			} else {
				resetRead();
				return false;
			}
		}
		bytesRead++;
	}
}

int32_t serialAvailable() {
	while (Serial.available() > 0) {
		if (processByte((byte)Serial.read())) {
			if (state == DONE) {
				resetRead();
				return bytesRead;
			}
		} else
			return -1;
		if (state > 1 && millis() - startRec > SERIAL_TIMEOUT) {
			resetRead();
			return -1;
		}
	}
	return -1;
}
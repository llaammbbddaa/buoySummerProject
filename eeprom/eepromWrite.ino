#include <EEPROM.h>
#include <stdint.h>

/**
 * Writes a 64-bit unsigned integer to EEPROM.
 * 
 * @param startAddr  The EEPROM address at which to begin writing (0–EEPROM_SIZE-8).
 * @param value      The uint64_t value to store.
 */
void writeUint64ToEEPROM(int startAddr, uint64_t value) {
  // Break the 64-bit value into 8 bytes, MSB first
  for (int i = 0; i < 8; i++) {
    byte b = (value >> (8 * (7 - i))) & 0xFF;
    EEPROM.write(startAddr + i, b);
  }
}

// Example usage in your sketch:
void setup() {
  Serial.begin(9600);

}

uint64_t overWrite = (uint64_t)(1234567890);

void loop() {
  

  for (int i = 0; i < 500; i++) {
    writeUint64ToEEPROM((i * 8), overWrite);
    Serial.println(i);
  }

  Serial.println("done");
  while(1);
}

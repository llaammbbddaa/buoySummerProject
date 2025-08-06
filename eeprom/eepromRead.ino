// script for reading all of the eeprom data
// saves even if arduino is unplugged or reset, which is good enough
// meant for arduino mega

#include <EEPROM.h>
#include <stdint.h>

//–– Helper: print a uint64_t in decimal via Serial ––
void printUint64(uint64_t x) {
  if (x == 0) {
    Serial.println('0');
    return;
  }
  char buf[21];
  buf[20] = '\0';
  int pos = 20;
  while (x > 0 && pos > 0) {
    buf[--pos] = '0' + (x % 10);
    x /= 10;
  }
  Serial.println(&buf[pos]);
}

//–– Rebuild a uint64_t from 8 big-endian bytes ––
uint64_t bytes8ToUint64(const byte* b) {
  uint64_t v = 0;
  for (int i = 0; i < 8; i++) {
    v = (v << 8) | b[i];
  }
  return v;
}

//–– Turn a uint64_t into a decimal String (if ever needed) ––
String uint64ToString(uint64_t x) {
  if (x == 0) return "0";
  char buf[21];
  buf[20] = '\0';
  int pos = 20;
  while (x > 0 && pos > 0) {
    buf[--pos] = '0' + (x % 10);
    x /= 10;
  }
  return String(&buf[pos]);
}

// read previous data, will be implemented later to extract data from mega
void getSection(int offset) {
  byte dataBytes[8];
  for (int i = 0; i < 8; i++) {
    dataBytes[i] = EEPROM.read(i + offset);
  }

  // outputs the reconstructed encoded int, will be decoded elsewhere eventually...
  Serial.println(String(offset) + ": " + uint64ToString(bytes8ToUint64(dataBytes)));
}

void setup() {
  Serial.begin(9600);
}

// begin reading
int offset = 0;
void loop() {

  getSection(offset);
  
  offset += 8;
  if (offset >= 4000) {
    while(1); // stop once reached the end of eeprom
  }

  
}

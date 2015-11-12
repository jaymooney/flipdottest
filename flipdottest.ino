#include <SoftwareSerial.h>


// 0x80 beginning 
//___________________
// 0x81 - 112 bytes / no refresh / C+3E
// 0x82 - refresh
// 0x83 - 28 bytes of data / refresh / 2C
// 0x84 - 28 bytes of data / no refresh / 2C
// 0x85 - 56 bytes of data / refresh / C+E
// 0x86 - 56 bytes of data / no refresh / C+E
// ---------------------------------------
// address or 0xFF for all
// data ... 1 to nuber of data bytes
// 0x8F end

// panel's speed setting: 1-OFF 2-ON 3 - ON
// panel address : 1 (8 pos dip switch: 1:on 2 -8: off)

// I was sng RS485 Breakout and Duemilanova connected in the following way:
// [Panel]  [RS485]  [Arduino]
// 485+       A  
// 485-       B
//          3-5V    5V
//          RX-I    TX
//          TX-O    Not connected
//           RTS    5V
//           GND    GND


byte dot0 = 1;
byte dot1 = 2;
byte dot2 = 4;
byte dot3 = 8;
byte dot4 = 16;
byte dot5 = 32;
byte dot6 = 64;

byte all = 127;
byte testpattern1 = dot1 | dot3 | dot5;
byte testpattern2 = ~testpattern1;
byte* buildFlippyDottys(byte address, bool meh);
  
// dark / bright transmissions for configurations above

byte all_bright_2C[]= {0x80, 0x83, 0x02, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x8F};

byte all_dark_2C[]= {0x80, 0x83, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8F};


byte* funpattern1 = buildFlippyDottys(1, false);
byte* funpattern1a = buildFlippyDottys(1, true);
byte* funpattern2 = buildFlippyDottys(2, true);
byte* funpattern2a = buildFlippyDottys(2, false);

byte all_bright_01[]= {0x80, 0x83, 0x01, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x8F};

byte all_dark_01[]= {0x80, 0x83, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8F};

byte* buildFlippyDottys(byte address, bool meh) {
  byte* fun = (byte*) malloc(sizeof(byte) * 32);
  fun[0] = 0x80;
  fun[1] = 0x83;
  fun[2] = address;
  for (int i = 3; i < 31; i++) {
    fun[i] = (i % 2 == meh) ? testpattern1 : testpattern2;
  }
  fun[31] = 0x8f;
  return fun;
}


int delayms = 2000;
int minrefresh = 120;

SoftwareSerial softSerial(8, 9);

void setup() {
  Serial.begin(57600);
  softSerial.begin(57600);
}



void loop() {

  softSerial.write(funpattern2, 32);
  softSerial.write(funpattern1, 32); 
   delay (minrefresh);
  softSerial.write(funpattern2a, 32);
  softSerial.write(funpattern1a, 32);
   delay (delayms);
} 

#include<Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

void setup() {
  Serial1.begin(31250);
  bmp.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

//int last = 0;
//int current = 0;

int note = 0;

void loop() {
//    for (int i = 0; i < 4; i++) {
//        if (digitalRead(i) == HIGH) {
//            current |= 1 << i;
//        }
//    }
//
//    if (current != last) { // && foukam
//         // send midi note
//    }
//
//    // send foukani

//Serial1.write(1);
  Serial1.write(0b10010100);
  Serial1.write(69);
  Serial1.write(map(bmp.readPressure(), 0, 2*2147483648, 1, 127));
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(400); 
}

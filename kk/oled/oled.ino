#include <Wire.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>
#include <DW1000.h>

SSD1306AsciiWire display;

void setup() {
  Serial.begin(9600);
  Serial.println("Serial Begin");

  Wire.begin();
  display.begin(&Adafruit128x64, 0x3C);
  display.setFont(System5x7); 
  display.clear();
  
  display.println("Tag"); 
}

void loop() {
}
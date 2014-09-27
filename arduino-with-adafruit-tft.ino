/***************************************************
  This is a library for the Adafruit 1.8" SPI display.

This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
The 1.8" TFT shield
  ----> https://www.adafruit.com/product/802
The 1.44" TFT breakout
  ----> https://www.adafruit.com/product/2088
as well as Adafruit raw 1.8" TFT display
  ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

/**
  Simple MP3 Player display.
  
  @author: Ulrich VACHON (2014)
*/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     10
#define TFT_RST    9  // you can also connect this to the Arduino reset. In which case, set this #define pin to 0!
#define TFT_DC     8

// Option 1 (recommended): must use the hardware SPI pins
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Option 2: use any pins but a little slower!
//#define TFT_SCLK 13   // set these to be whatever pins you like!
//#define TFT_MOSI 11   // set these to be whatever pins you like!
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

#define BUTTON_NONE 0
#define BUTTON_DOWN 1
#define BUTTON_RIGHT 2
#define BUTTON_SELECT 3
#define BUTTON_UP 4
#define BUTTON_LEFT 5

#define BACKGROUND_COLOR 0x07FF

#define FONT_COLOR 0x0111

boolean running = false;

char* trackList[] = {"Prowler.mp3", "Hells Bells.mp3", "Any track.mp3"};

int trackIndex = 0;

void setup(void)
{
  Serial.begin(9600);

  Serial.println(F("[DEBUG] - Display used for Ardunio Music Player based on Adafruit ST7735 shield"));

  tft.initR(INITR_BLACKTAB);

  Serial.println(F("[DEBUG] - ST7735 found"));

  refresh();
}

void loop()
{
  uint8_t button = readButton();

  if (BUTTON_SELECT == button)
  {
    running = !running;
  }
  if (BUTTON_DOWN == button)
  {
    ++trackIndex;
  }
  if (BUTTON_UP == button)
  {
    --trackIndex;
  }
  refresh();

  delay(100);
}

void refresh()
{
  tft.fillScreen(BACKGROUND_COLOR);

  drawTitle();

  drawStatus();

  drawTrackList();
}

void drawTitle()
{
  drawString("Arduino Music Player", 5, 5, FONT_COLOR);

  tft.drawLine(0, 20, tft.width(), 20, FONT_COLOR);
}

void drawStatus()
{
  if (running)
  {
    drawString("Playing track...", 5, 30, FONT_COLOR);
  }
  else {
    drawString("Stop...", 5, 30, FONT_COLOR);
  }
}

void drawTrackList()
{
  // use markup var instead of

  for (byte i = 0, j = 0; i < 3; i++, j = j + 10)
  {
    if (i == trackIndex)
    {
      drawString(">", 5, (45 + j), FONT_COLOR);
    }
    else
    {
      drawString(" ", 5, (45 + j), FONT_COLOR);
    }
    drawString(trackList[i], 12, (45 + j), FONT_COLOR);
  }
}

void drawString(char *text, int16_t positionX, int16_t positionY, uint16_t color)
{
  tft.setCursor(positionX, positionY);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

uint8_t readButton(void)
{
  float a = analogRead(3);

  a *= 5.0;
  a /= 1024.0;

  Serial.print("Button read analog = ");
  Serial.println(a);

  if (a < 0.2) return BUTTON_DOWN;
  if (a < 1.0) return BUTTON_RIGHT;
  if (a < 1.5) return BUTTON_SELECT;
  if (a < 2.0) return BUTTON_UP;
  if (a < 3.2) return BUTTON_LEFT;
  else return BUTTON_NONE;
}


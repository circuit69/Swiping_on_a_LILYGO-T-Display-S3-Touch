/*********************************************************
   Lilygo T-Display-S3 Touch minimal code test
   My barebones test sketch
*********************************************************/
#define TOUCH_MODULES_CST_SELF
#include "TFT_eSPI.h"
#include "TouchLib.h"
#include "Wire.h"
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite scrn = TFT_eSprite(&tft);

//some of the more important pins on the t-display-s3
#define PIN_BUTTON_1                 0
#define PIN_BUTTON_2                 14
#define PIN_BAT_VOLT                 4
#define PIN_LCD_BL                   38
#define PIN_POWER_ON                 15
#define PIN_IIC_SCL                  17
#define PIN_IIC_SDA                  18
#define PIN_TOUCH_INT                16
#define PIN_TOUCH_RES                21

//Initialize the touch screen
TouchLib touch(Wire, PIN_IIC_SDA, PIN_IIC_SCL, CTS820_SLAVE_ADDRESS, PIN_TOUCH_RES);

void setup(void) {
  //The two buttons on the board (setup but not used in this sketch)
  pinMode(PIN_BUTTON_1, INPUT);
  pinMode(PIN_BUTTON_2, INPUT);

  //Needed to power TFT on battery
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);

  Wire.begin(PIN_IIC_SDA, PIN_IIC_SCL);   //needed for the touch screen
  tft.init();                   //setup the tft display
  tft.setRotation(3);           //USB on left
  //the touch screen is not rotated, you need to handle orientation yourself in code
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_SILVER, TFT_BLACK);
}

void loop() {
  if (touch.read()) {   //check if the screen is touched
    TP_Point t = touch.getPoint(0);   //load the x,y coordinates
    tft.fillScreen(TFT_BLACK);
    tft.drawCentreString("Raw", 80, 20, 4);         //raw touch screen coordinates
    tft.drawCentreString("X=" + String(t.x), 80, 60, 4);  //t.x is the value from the touch screen
    tft.drawCentreString("Y=" + String(t.y), 80, 100, 4); //t.y value from the touch screen
    tft.drawCentreString("Oriented", 240, 20, 4);   //reoriented touch screen coordinates
    tft.drawCentreString("X=" + String(320 - t.y), 240, 60, 4);
    tft.drawCentreString("Y=" + String(t.x), 240, 100, 4);
  }
  delay(10);
}

/*********************************************************
   Lilygo T-Display-S3 Touch swiping example
   let's push a dot around the screen
*********************************************************/
#define TOUCH_MODULES_CST_SELF
//#include "Arduino.h"
#include "TFT_eSPI.h"
#include "TouchLib.h"
#include "Wire.h"
//#include <TFT_eSPI.h>
//#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite scrn = TFT_eSprite(&tft);

#define PIN_BUTTON_1                 0
#define PIN_BUTTON_2                 14
#define PIN_BAT_VOLT                 4
#define PIN_LCD_BL                   38
#define PIN_POWER_ON                 15
#define PIN_IIC_SCL                  17
#define PIN_IIC_SDA                  18
#define PIN_TOUCH_INT                16
#define PIN_TOUCH_RES                21

TouchLib touch(Wire, PIN_IIC_SDA, PIN_IIC_SCL, CTS820_SLAVE_ADDRESS, PIN_TOUCH_RES);

int x = 50, lastX = x, dirX,    //dot variables
    y = 50, lastY = y, dirY,
    drag = 2, minSwipe = 20,
    tx1, ty1, tx2, ty2;         //touch screen variables

bool touching = false;

void setup(void) {
  //The two buttons on the board
  pinMode(PIN_BUTTON_1, INPUT);
  pinMode(PIN_BUTTON_2, INPUT);

  //Needed to power TFT on battery
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);

  tft.init();
  tft.setRotation(3);         //USB on left
  //tft.fillScreen(TFT_BLACK);
  scrn.createSprite(320, 170);
  //scrn.fillSprite(TFT_BLACK);
  Wire.begin(PIN_IIC_SDA, PIN_IIC_SCL);
}

void loop() {
  if (touch.read()) {
    TP_Point t = touch.getPoint(0);
    if (!touching) {
      touching = true;
      tx1 = t.y;        //first touch
      ty1 = -t.x;
    } else {
      tx2 = t.y;        //last touch
      ty2 = -t.x;
    }
  } else if (touching) {  //touch lifted
    touching = false;
    dirX = tx1 - tx2;
    dirY = ty1 - ty2;
  }
  moveBall();
  showScreen();
  delay(10);
}

void showScreen() {
  scrn.fillSprite(TFT_BLACK);
  scrn.fillCircle(x, y, 4, TFT_CYAN);
  scrn.pushSprite(0, 0);
}

void moveBall() {
  if (x < -dirX || x > 320 - dirX) { //hit side of screen left and right
    dirX = -dirX;
  }
  if (y < -dirY || y > 170 - dirY) { //hit side of screen top and bottom
    dirY = -dirY;
  }
  if (dirX < 0) {   //slow the ball moving left
    dirX += drag;
    if (dirX > 0) {
      dirX = 0;
    }
  }
  if (dirX > 0) {   //slow the ball moving right
    dirX -= drag;
    if (dirX < 0) {
      dirX = 0;
    }
  }
  if (dirY < 0) {   //slow the ball moving up
    dirY += drag;
    if (dirY > 0) {
      dirY = 0;
    }
  }
  if (dirY > 0) {   //slow the ball moving down
    dirY -= drag;
    if (dirY < 0) {
      dirY = 0;
    }
  }
  x += dirX;
  y += dirY;
}

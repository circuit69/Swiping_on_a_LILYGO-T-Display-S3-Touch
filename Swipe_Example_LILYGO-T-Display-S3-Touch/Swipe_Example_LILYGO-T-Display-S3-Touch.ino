/*********************************************************
   Lilygo T-Display-S3 Touch swiping example
   let's push a dot around the screen
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

int x = 50, dirX,    //dot variables
    y = 50, dirY,
    drag = 2,
    tx1, ty1,         //first touch vairiable
    tx2, ty2;         //last touch variables when lifting

bool touching = false;          //flag if screen is activly touched

void setup(void) {
  //The two buttons on the board (setup but not used in this sketch)
  pinMode(PIN_BUTTON_1, INPUT);
  pinMode(PIN_BUTTON_2, INPUT);

  //Needed to power TFT on battery
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);

  tft.init();                   //setup the tft display
  tft.setRotation(3);           //USB on left
  //the touch screen is not rotated, you need handle orientation yourself in code
  scrn.createSprite(320, 170);  //a sprite for smooth graphics
  Wire.begin(PIN_IIC_SDA, PIN_IIC_SCL);   //needed for the touch screen
}

void loop() {
  if (touch.read()) {   //check if the screen is touched
    TP_Point t = touch.getPoint(0);   //load the x,y coordinates
    if (!touching) {    //check if the flag "touching" is false
      touching = true;  //the screen is actively touched
        //this is handling the orientation of the touch screen to the display
        //tx1 is the display orientation = t.y the touchscreen orientation
      tx1 = t.y;        //first touch coordinates
        //ty1 is the display orientation = t.x the touchscreen reversed orientation
      ty1 = -t.x;
    } else {            //if "touching" is true
      tx2 = t.y;        //get the last coordinates
      ty2 = -t.x;
    }
  } else if (touching) {  //touch lifted, is "touching" true
    touching = false;     //then make it false
    dirX = tx1 - tx2;     //how long was the swipe simple calculation
    dirY = ty1 - ty2;
  }
  moveBall();
  showScreen();
  delay(10);
}

void showScreen() {
  scrn.fillSprite(TFT_BLACK);           //clear the sprite
  scrn.fillCircle(x, y, 4, TFT_CYAN);   //draw a dot on the sprite
  scrn.pushSprite(0, 0);                //push the sprite to the tft screen
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
  x += dirX;        //add directions to move the ball
  y += dirY;
}

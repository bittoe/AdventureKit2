/*
 * Day 0 - AI Apocalypse by inventr.io
 * Learn more at https://inventr.io/PLACEHOLDER
 *
 * Getting Started
 * HiLetgo 2.4" ILI9341 240X320 TFT LCD Display with Touch Panel
 * Touch Switch with Rotation
 *
 * The LCD display can be rotated so that any of it's four sides are designated
 * as "top".  Rotations are numbered 0-4 with rotation 0 used when the display
 * is oriented with the white reset button on top.  The remaining rotations
 * proceed in a clockwise direction (as will be demonstrated with our first test
 * program).
 *
 * Rotations:
 *
 * 0. Portrait rotation (white button on top)
 * 1. Landscape rotation (white button on left)
 * 2. Portrait rotation (white button on bottom)
 * 3. Landscape rotation (white button on right)
 *
 * Adapted for Inventr.io by David Schmidt
 */

// #include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

// Define minimum and maximum pressure 
constexpr uint16_t MINPRESSURE = 200;
constexpr uint16_t MAXPRESSURE = 1000;

// Replace these lines with the two constexpr lines you generated (and saved)
// from when you ran the Touch Screen Calibration program (2-TouchScreenCalibration).
//=========================================
constexpr int XP=8, XM=A2, YP=A3, YM=9; //240x320 ID=0x9341
constexpr int TS_LEFT=110,TS_RT=916,TS_TOP=84,TS_BOT=907;
//=========================================

// Rotations 0,2 = portrait  : 0->USB=right,upper : 2->USB=left,lower
// Rotations 1,3 = landscape : 1->USB=left,upper  : 3->USB=right,lower
byte rotation = 0; //(0->3)

// Define LCD display and touch panel objects
MCUFRIEND_kbv tft;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Define objects for on-screen graphical buttons
Adafruit_GFX_Button on_btn, off_btn;

int pixel_x, pixel_y;     // Touch_getXY() updates global vars
bool Touch_getXY(void)
{
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);      // restore shared pins
  pinMode(XM, OUTPUT);      // because TFT control pins
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  if (pressed) {
    // Once we have the raw touch coordinates they need to be rotated to match the
    // current screen rotation AND scaled to match the graphics dimensions.  For
    // each rotation we select the proper mapping and scaling values.
    switch (rotation) {
    case 0:
      pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());
      pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
      break;
    case 1:
      // p.x, p.y reversed //
      pixel_x = map(p.y, TS_TOP, TS_BOT, 0, tft.width());
      pixel_y = map(p.x, TS_RT, TS_LEFT, 0, tft.height());
      break;
    case 2:
      pixel_x = map(p.x, TS_RT, TS_LEFT, 0, tft.width());
      pixel_y = map(p.y, TS_BOT, TS_TOP, 0, tft.height());
      break;
    case 3:
      //  p.x, p.y reversed //
      pixel_x = map(p.y, TS_BOT, TS_TOP, 0, tft.width()); 
      pixel_y = map(p.x, TS_LEFT, TS_RT, 0, tft.height());
      break;
    }
  }
  return pressed;
}

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup(void)
{
  Serial.begin(115200);

  uint16_t ID = tft.readID(); // Retrieve ID of LCD/touch panel
  Serial.print("TFT ID = 0x");
  Serial.println(ID, HEX);

  // Display red/green "light" indicator and buttons, centered for our
  // specified orientation (rotation).
  tft.begin(ID);
  tft.setRotation(rotation);
  tft.fillScreen(BLACK);
  int LIGHT_WIDTH = (tft.width() * 2) / 3;
  int LIGHT_HEIGHT = tft.height() / 4;
  int LIGHT_X_PAD = (tft.width() - LIGHT_WIDTH) / 2;
  tft.fillRect(LIGHT_X_PAD, LIGHT_HEIGHT, LIGHT_WIDTH, LIGHT_HEIGHT, RED);
  // tft.fillRect(40, 80, 160, 80, RED);
  int BTN_X_PAD = tft.width() / 12; // 20
  int BTN_WIDTH = BTN_X_PAD * 5; // 100
  int BTN_Y_PAD = tft.height() / 8; // 40
  int BTN_HEIGHT = tft.height() / 8;  // 40
  Serial.println(BTN_X_PAD);
  Serial.println(BTN_WIDTH);
  Serial.println(BTN_Y_PAD);
  Serial.println(BTN_HEIGHT);
  // initButton(&GFX, X_CENTER, Y_CENTER, WIDTH, HEIGHT, ...)
  // initButton(&GFX, X, Y, WIDTH, HEIGHT, ...)
  on_btn.initButton(&tft, BTN_X_PAD * 3, tft.height() / 2 + BTN_Y_PAD, BTN_WIDTH, BTN_HEIGHT, WHITE, CYAN, BLACK, "ON", 2);
  // on_btn.initButton(&tft,  60, 200, 100, 40, WHITE, CYAN, BLACK, "ON", 2);
  // off_btn.initButton(&tft, 180, 200, 100, 40, WHITE, CYAN, BLACK, "OFF", 2);
  off_btn.initButton(&tft, 180, 200, 100, 40, WHITE, CYAN, BLACK, "OFF", 2);
  on_btn.drawButton(false);
  off_btn.drawButton(false);
}

/* two buttons are quite simple
 */
void loop(void)
{
    bool down = Touch_getXY();
    on_btn.press(down && on_btn.contains(pixel_x, pixel_y));
    off_btn.press(down && off_btn.contains(pixel_x, pixel_y));
    if (on_btn.justReleased())
        on_btn.drawButton();
    if (off_btn.justReleased())
        off_btn.drawButton();
    if (on_btn.justPressed()) {
        on_btn.drawButton(true);
        tft.fillRect(40, 80, 160, 80, GREEN);
    }
    if (off_btn.justPressed()) {
        off_btn.drawButton(true);
        tft.fillRect(40, 80, 160, 80, RED);
    }
    delay(250);
}

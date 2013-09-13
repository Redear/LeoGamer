// Pins SCLK and MOSI are fixed in hardware, and pin 10 (or 53) 
// must be an output
//#define sclk 13    // for MEGAs use pin 52
//#define mosi 11    // for MEGAs use pin 51
#define cs 10   // for MEGAs you probably want this to be pin 53
#define dc 9
#define rst 8  // you can also connect this to the Arduino reset


#include <ST7735.h>
#include <SPI.h>
#include "Color.h"
// Option 1: use any pins but a little slower
//ST7735 tft = ST7735(cs, dc, mosi, sclk, rst);  

// Option 2: must use the hardware SPI pins 
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be 
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
ST7735 tft = ST7735(SS, dc, rst);
float p = 3.141592; // value for pi
int tft_width = 128;
int tft_height = 160;
int btn0 = 10;
int btn1 = 11;
int radius = 12;
int mode = 0;
int num_modes = 3;
Color cur_color(215,215,215);

void setup(void) {
  tft.initR();               // initialize a ST7735R chip

  tft.writecommand(ST7735_DISPON);
  int bg = cur_color.get_color();
  tft.fillScreen(bg);
  delay(700);
  pinMode(btn0,INPUT);
  pinMode(btn1,INPUT);

}
int cur_r = 0;
int cur_g = 0;
int cur_b = 0;
int cur_x = 60;
int cur_y = 60;
void loop() {
  float h = analogRead(0) ;
  float s = analogRead(1) ;
  float b = 700 ;
  h /= 1024.0;
  s /= 1024.0;
  b /= 1024.0;
  cur_color.convert_hcl_to_rgb(h,s,b);
  int color = cur_color.get_color(); 
  delay(10);
  if (mode == 2 ) radius = analogRead(2) >> 4; 
  joy_draw(color);
 
  if ( digitalRead(btn1) == HIGH) {
    mode = ++mode % num_modes;
    delay(500);
  } 
  if ( digitalRead(btn0) == HIGH) {
    tft.fillScreen(~color);
    delay(500);
  } 
}
void joy_draw(int color){
  int xraw = analogRead(0);
  int yraw = 1024-analogRead(1);
  if (cur_x < tft_width -radius && xraw > 600) cur_x++;
  else if ( cur_x > radius  && xraw < 360 ) cur_x--; 
  if ( cur_y < tft_height - radius && yraw > 600) cur_y++;
  else if ( cur_y > radius  && yraw < 360 ) cur_y--; 
  if ( mode == 0 )tft.fillCircle(cur_x, cur_y, radius/2,color);
  else tft.fillRect(cur_x, cur_y, radius, radius,color);
}


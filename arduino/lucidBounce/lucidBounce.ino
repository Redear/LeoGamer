/*
File: lucidBounce.ino
************************************************************
Bounce is a Pong like game that gives the player a bouncing
ball which increases with speed after every bounce off the
control paddle.

This file uses the adafruit library ST7735 which can be
found at this url: XXXXXXXXXXXXXXXXXXXXXX

	
*/

// Pins SCLK and MOSI are fixed in hardware, and pin 10 (or 53) 
// must be an output
//#define sclk 13    // for MEGAs use pin 52
//#define mosi 11    // for MEGAs use pin 51
#define cs 10   // for MEGAs you probably want this to be pin 53
#define dc 9
#define rst 8  // you can also connect this to the Arduino reset

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

#include <ST7735.h>
#include <SPI.h>

// Option 1: use any pins but a little slower
//ST7735 tft = ST7735(cs, dc, mosi, sclk, rst);  

// Option 2: must use the hardware SPI pins 
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be 
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)

// sc ----> SS wire difference SLAVE SELECT
ST7735 tft = ST7735(SS, dc, rst);

//Create constants as such
float p = 3.141592; // value for pi
int tft_width = 128;
int tft_height = 160;
int x1 = 60;
int y1 = 60;
int x2 = 20;
int y2 = 80;
int ballx = 60;
int bally = 60;
int deltay = 1;
int deltax = 1;

int ball_speed = 24;


int left_leds[] = {0,1,2};
int left_score = 3;

int right_leds[] = {5,6,7};
int right_score = 3;

int background_color = 0x00;
void setup(void) {
  tft.initR();               // initialize a ST7735R chip

  tft.writecommand(ST7735_DISPON);
  tft.fillScreen(background_color);
  delay(700);
  for(int i = 0; i < 3; i++){
    pinMode(left_leds[i],OUTPUT);
    pinMode(right_leds[i],OUTPUT);

  }
  for(int i = 0; i < 3; i++){
    digitalWrite(left_leds[i],HIGH);
    digitalWrite(right_leds[i],HIGH);

  }
  delay(500);
  
}

void loop() {  
  int cur_r = random(20,32);
  int cur_g = random(55,64);
  int cur_b = random(10,32);
  int color = cur_r  | (cur_g << 5) | (cur_b << 11); 
  int radius = 4;
  draw_paddles();
  draw_ball(color);
  delay(ball_speed);
}
void draw_input_text(){ 
  int cur_x2 =  analogRead(2);
  int cur_y2 =  analogRead(3);  
  String xs = String(cur_x2);
  String ys = String(cur_y2);
  char cx[4];
  cx[0] = xs.charAt(0);
  cx[1] = xs.charAt(1);
  cx[2] = xs.charAt(2);
  cx[3] =0;
  char cy[4];
  cy[0] = ys.charAt(0);
  cy[1] = ys.charAt(1);
  cy[2] = ys.charAt(2);
  cy[3] =0;
  tft.fillRect(10,20,40,40,background_color);
  tft.drawString(12,22,cx ,YELLOW);
  tft.drawString(12,40,cy ,YELLOW);
}
void draw_paddles(){
  int cur_x =  analogRead(3);
  int cur_y =  1024 - analogRead(2);  
  int hval = 620;
  int lval = 350;
 
  if ( y1 < tft_height - 20  && cur_y > hval) y1++;
  else if ( y1 > 0 && cur_y < lval ) y1--; 
 
  int cur_x2 =  analogRead(1);
  int cur_y2 = 1024 - analogRead(0); 

  if ( y2 < tft_height - 20 && cur_y2 > hval ) y2++;
  else if (y2 > 0 && cur_y2 < lval ) y2--; 
  
  tft.fillRect(2, y1-3, 5, 27, background_color);
  tft.fillRect(3, y1, 3, 20, YELLOW);
  tft.fillRect(tft_width-3, y2-3, 5, 27,background_color);
  tft.fillRect(tft_width-3, y2, 3, 20, YELLOW);
}
void draw_ball(int color){
   int bradius = 6;
    tft.fillCircle(ballx, bally, bradius , color);
    ballx += deltax;
    bally += deltay;
    if ( ballx < 7 && bally > y1 && bally < y1+20) deltax *= -1;
    else if ( ballx > tft_width-7 && bally > y2 && bally < y2+20) deltax *= -1;
    else if ( bally <= bradius || bally >= tft_height - bradius ) deltay *= -1;
    if (ballx <=  0 ) {
      left_score--;
      for(int i = 0; i < 3; i++){
        digitalWrite(left_leds[i],LOW);
      }
      delay(200);
      for(int i = 0; i < left_score; i++){
        digitalWrite(left_leds[i],HIGH);
      }
  
      int cur_r = random(0,26);
      int cur_g = random(0,30);
      int cur_b = random(0,18);
      background_color = cur_r  | (cur_g << 5) | (cur_b << 11); 
      tft.fillScreen(background_color);
      tft.drawString(2,42, "Right Won!" ,~background_color,2);
      tft.drawString(2,62, "Get Ready!" ,CYAN,2);
      for (int i = 0; i <250; i++){
        delay(10);
        draw_paddles();
      }
      
      ballx = 64;
      bally = 80;
      if (ball_speed > 1) ball_speed--;
    } else if (ballx >= tft_width  ) {
      right_score --;
      for(int i = 0; i < 3; i++){
        digitalWrite(right_leds[i],LOW);
      }
      delay(200);
      for(int i = 0; i < right_score; i++){
        digitalWrite(right_leds[i],HIGH);
      }
      int cur_r = random(0,26);
      int cur_g = random(0,30);
      int cur_b = random(0,18);
      background_color = cur_r  | (cur_g << 5) | (cur_b << 11); 
      tft.fillScreen(background_color);
      tft.drawString(2,42, "Left Won!" ,YELLOW,2);
      tft.drawString(2,62, "Get Ready!" ,GREEN,2);
      for (int i = 0; i <250; i++){
        delay(10);
        draw_paddles();
      }
      ballx = 64;
      bally = 80;
      if (ball_speed > 1) ball_speed--;
    }
}

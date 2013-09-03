/*
File: lucidBounce.ino
************************************************************ 
    lucidBounce.ino is free software: you can redistribute it and/or modify
    it under the terms of the GNU version 3 General Public License as 
    published by the Free Software Foundation.  

    lucidBounce.ino is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with lucidBounce.  If not, see <http://www.gnu.org/licenses/>.
*************************************************************
*************************************************************
lucidBounce is a Pong like game that gives the player a bouncing
ball which increases with speed  and number of balls after every 
five bounces off the control paddle.
Check out the tutorial for this game on our website:
URL FOR TUTORIAL: http://www.lucidtronix.com/tutorials/55
*************************************************************
LIBRARIES USED:
*************************************************************
ARDUINO SPI.h
Adafruit library ST7735 which can be
found at this url: https://github.com/adafruit/Adafruit-ST7735-Library
*************************************************************
LINES AND WIRES:
*************************************************************
joystick read lines:
digitalRead(12)
int cur_x =  analogRead(0);
int cur_y =  1024 - analogRead(2);  
int cur_y =  1024 - analogRead(1);

cs 10   // for MEGAs you probably want this to be pin 53
dc 9
rst 8  // you can also connect this to the Arduino reset
	
*/
#define DEBUG
#ifdef DEBUG
  #define DEBUG_PRINT(x)  Serial.println (x)
#else
  #define DEBUG_PRINT(x)
#endif

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
const float PIE = 3.141592; // value for pi
const int tft_width =128;   //128;
const int tft_height =160;   //160;
const int paddle_width= 21;
const int paddle_height=3;

int level[]={50,150,250};
int ball_radius=3;
int bounce_count=1;
int paddle_x= 60;
int paddle_y= 152;
int num_balls=1;
float ballx []= {60.0,10.0,20.0};
float bally []= {60.0,80.0,77.0};
float deltay[] = {1.0,1.0,1.0};
float deltax[] = {.5,.5,.5};

/*TODO: points 
at 50 points you go to level 2 (2 balls)
at 150 points you go to level 3 (3 balls)
*/
int points = 0;  
int ball_speed = 24;
int background_color =BLACK;

/*function: setup(void)
  ****************************************
  The typical setup method for arduino programs.
  Here we initialize a TFT screen. 
*/
void setup(void) {
  Serial.begin(9600);
  DEBUG_PRINT ("I'm here");
  tft.initR();               // initialize a ST7735R chip
  tft.writecommand(ST7735_DISPON);
  tft.fillScreen(background_color);
  delay(1500); 
}
int cur_r = random(20,32);
int cur_g = random(55,64);
int cur_b = random(10,32);

/*function: loop()
  ***************************************
  Arduino event loop function which runs continuously on the chip
  while power is delivered.  Functions for balls and paddle are called 
  repeatedly by this function.

*/
void loop() {  
  int color = cur_r  | (cur_g << 5) | (cur_b << 11);
  draw_paddle();
  if (points > level[num_balls-1]) num_balls++;
  draw_balls(num_balls);
  delay(ball_speed);
}

/*function: draw_input_text
  usage: 
*/
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
/*
  function: draw_paddle
  usage:  draw_paddle();
  **********************************************
  draws the bounce paddle to the screen using analogRead for current x and y
  working on a two potentiameter joystick that reads analog values from 0 - 1024
  525 is straight up and down.  
  tft.fillRect(topLeftX,topLeftY,width, height, color);
*/
void draw_paddle(){
  int cur_x =  analogRead(0);
  int cur_y =  1024 - analogRead(1);
  int rightVal = 620; //what the hell are these
  int leftVal = 350;
  if ( paddle_x+15 < tft_width  && cur_x > rightVal) paddle_x+=3;//move paddle right
  else if ( paddle_x > 0 && cur_x < leftVal ) paddle_x-=3; //move paddle left  
  tft.fillRect(paddle_x-5, paddle_y-5, paddle_width*2, paddle_height+5, background_color);
  tft.fillRect(paddle_x, paddle_y, paddle_width, paddle_height, YELLOW);
 // DEBUG_PRINT("current x =");
 // DEBUG_PRINT(cur_x);
}


/*  function: draw_ball
    usage:  draw_ball(color, radius);
    *************************************************
    draws the ball to the screen.  Also, controls ball logic for bouncing off the wall
    and winning conditions
    tft.fillCircle(centerX,centerY,radius,color);
*/
void draw_balls( int number_balls){
     int bradius = 3;
     int color[] = {RED,BLUE,YELLOW,GREEN};     
     for ( int i=0 ; i < number_balls; i++){     
      tft.fillCircle(ballx[i],bally[i], bradius+3, background_color);
      tft.fillCircle(ballx[i], bally[i], bradius , color[i]);     
      ballx[i] += deltax[i];
      bally[i] += deltay[i];      
     /* ball velocity delta's   */   
    if (ballx[i] > tft_width -1) deltax[i] *= -1;
    else if (ballx[i] <=1) deltax[i] *= -1;
    else if( ballx[i] >= paddle_x && ballx[i] <= paddle_x+paddle_width && bally[i]> paddle_y-bradius){ 
      deltay[i] *= -1.1;
      deltax[i] *= 1.1;
      points+=10;

      }
      else if (bally[i] < 4 ) {
        DEBUG_PRINT("ball y=");
        DEBUG_PRINT(bally[i]);
        deltay[i] *= -1;
      }
       // losing condition   
      else if(bally[i] >  paddle_y ) {
        int cur_r = random(0,26);
        int cur_g = random(0,30);
        int cur_b = random(0,18);
        background_color = cur_r  | (cur_g << 5) | (cur_b << 11); 
        tft.fillScreen(background_color);
        tft.drawString(2,42, "YOU LOSE!" ,~background_color,2);
        tft.drawString(2,62, "Get Ready!" ,CYAN,2);
        deltax[i]= random(-1,1)+.5;
        deltay[i]= random(0,1)?  +.5: 1;
   
        delay(1000);
        //draw_paddle();     
        ballx[i] = 64.0;
        bally[i] = 10.0;
      }
     }
}

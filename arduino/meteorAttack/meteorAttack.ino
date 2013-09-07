/*
File meteorAttack.ino
***********************************************************
    meteorAttack.ino is free software: you can redistribute it and/or modify
    it under the terms of the GNU version 3 General Public License as 
    published by the Free Software Foundation.  

    meteorAttack.ino is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with meteorAttack.  If not, see <http://www.gnu.org/licenses/>.
*************************************************************
a clone of the classic game Missile Command for the arduino
Leonardo architecture.  This game uses a single button to
control multiple cannons that are tasked with protecting the
planet Choctaw from being destroyed by the missile onslaught of
the evil Emporer Georgenon Shrubins.
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
**************************************************************
Game Components:
**************************************************************
Cities: 3 cities Queens, Brooklyn, and The Bronx
Canons: 2 Canons 
Meteors: Lots of meteors (poisson random maybe)
Bugs: ? Haven't decided yet

*/

#define DEBUG
#ifdef DEBUG
  #define DEBUG_PRINT(x)  Serial.println (x)
#else
  #define DEBUG_PRINT(x)
#endif

//Line definitions
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
#include "Target.h"

// sc ----> SS wire difference SLAVE SELECT
ST7735 tft = ST7735(SS, dc, rst);


/*
  Global Constants 
*/
const int tft_width = 128;
const int tft_height = 160;
const int background_color =BLACK;
const int BROOKLYN_COLOR= CYAN;
const int QUEENS_COLOR = MAGENTA;
const int BRONX_COLOR = YELLOW;
const int CANON_COLOR = RED;
const int METEOR_COLOR = GREEN;
const int TARGET_COUNT = 5;//can vary if the number of targets is changed
const int CITY_HEIGHT = 12;
const int CANON_HEIGHT = 8;

int xScaled=60;
int yScaled=76;
int targetX=0;
int targetY=0;
int canonFireX=0;
int canonFireY=0;
int btnPin0 = 12;
unsigned int lastPress= 0;

/*Target: 
  x and y are static constants
  strength depends on type of Target
  width and height can be changed to allow for other
  shapes after implemented with rectangles first
*/
Target brooklyn(0.0,tft_height-CITY_HEIGHT,25,CITY_HEIGHT,4);
Target canon0(26.0,tft_height-CANON_HEIGHT,12,CANON_HEIGHT,2);
Target queens(50.0,tft_height-CITY_HEIGHT,25,CITY_HEIGHT,4);
Target canon1(76.0,tft_height-CANON_HEIGHT,12,CANON_HEIGHT,2);
Target bronx(100.0,tft_height-CITY_HEIGHT,25,CITY_HEIGHT,4);

Target targets[5]={brooklyn,canon0,queens,canon1,bronx};

/*Meteor: 
  startX at a position between 0 and screenwidth
  startY is just above 0 (negative number?)
  deltaX & deltaY depend on the targetIndex (ie: position of target)
  use random number to give startX ie: int cur_r = random(20,32);
*/
/*
struct Meteor{
  float startX, startY;
  float deltaX, deltaY;
  int radius;
  int targetIndex;
};
*/

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
  tft.drawString(4,4,"Starting...",CYAN,1);
  delay(1500); 
}

/*function: loop()
  ***************************************
  Arduino event loop function which runs continuously on the chip
  while power is delivered.  Functions for balls and paddle are called 
  repeatedly by this function.
*/
void loop() { 
  drawTargets();
  drawCanonSite();
 // int startX = cur_r  | (cur_g << 5) | (cur_b << 11);
  //delay(10);
}

void drawCanonSite(){
  int xRaw =  analogRead(0);
  int yRaw =  analogRead(1);
  int minVal = 0;
  int maxVal = 1024;
  int xDiff;
  int yDiff;
  xDiff = map(xRaw,minVal,maxVal,-1,2);
  yDiff = map(yRaw,minVal,maxVal,2,-2);
  tft.fillRect(targetX-6,targetY+3,6,1,background_color);
  tft.fillRect(targetX+3,targetY-6,1,6,background_color);

  targetX+= xDiff;
  targetY+= yDiff;
  targetX = constrain(targetX,5,tft_width-5);
  targetY = constrain(targetY,5,tft_width-5);
  tft.fillRect(targetX-6,targetY+3,6,1,BLUE);
  tft.fillRect(targetX+3,targetY-6,1,6,BLUE);
  if (digitalRead(btnPin0) && millis()-lastPress >400){
    canonFireX=targetX;
    canonFireY=targetY;
    //shootCanon(int x, int y);
    tft.fillCircle(canonFireX,canonFireY,5,GREEN);
  }
  DEBUG_PRINT(xRaw);
  DEBUG_PRINT(yRaw);
  DEBUG_PRINT(targetX);
  DEBUG_PRINT(targetY);
  DEBUG_PRINT(xDiff);
  DEBUG_PRINT(yDiff);
}


void drawTargets(){
  //Target *oneTarget;
  
  for (int i = 0; i< TARGET_COUNT;i++){
    if ( i % 2 == 0){
         tft.fillRect(targets[i].x, targets[i].y, targets[i].width, targets[i].height, QUEENS_COLOR);
    }else {
      tft.fillRect(targets[i].x, targets[i].y, targets[i].width, targets[i].height, CANON_COLOR);
    }
    
  }
} 

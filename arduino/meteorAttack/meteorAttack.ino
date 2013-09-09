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
const int BG_COLOR =BLACK;
const int BROOKLYN_COLOR= CYAN;
const int QUEENS_COLOR = MAGENTA;
const int BRONX_COLOR = YELLOW;
const int CANON_COLOR = RED;
const int METEOR_COLOR = GREEN;
const int TARGET_COUNT = 5;//can vary if the number of targets is changed
const int CITY_HEIGHT = 12;
const int CITY_WIDTH = 25;
const int CANON_HEIGHT = 8;
const int CANON_WIDTH = 4;
const int CITY_HIT_STRENGTH = 6;
const int CANON_HIT_STRENGHT = 2;
const int METEOR_MAX= 20;
const int TARGET_TOTAL = 5;
const int LOOP_SPEED = 24;
const int SHELLS_TOTAL = 20;

int xScaled=60;
int yScaled=76;
int targetX=0;
int targetY=0;
int canonFireX=0;
int canonFireY=0;
int btnPin0 = 12;
int meteorNum=0;

float meteorDY= 0.4;
int meteorCount= 0;
Target meteorArray[METEOR_MAX];
Target canonShells[SHELLS_TOTAL];
unsigned int lastPress= 0;
Target ally();
Target enemy();
/*Target: 
  x and y are static constants
  strength depends on type of Target
  width and height can be changed to allow for other
  shapes after implemented with rectangles first
*/
Target brooklyn(0.0,tft_height-CITY_HEIGHT,CITY_WIDTH,CITY_HEIGHT,CITY_HIT_STRENGTH);
Target canon0(35.0,tft_height-CANON_HEIGHT,CANON_WIDTH,CANON_HEIGHT,CANON_HIT_STRENGHT);
Target queens(50.0,tft_height-CITY_HEIGHT,CITY_WIDTH,CITY_HEIGHT,CITY_HIT_STRENGTH);
Target canon1(85.0,tft_height-CANON_HEIGHT,CANON_WIDTH,CANON_HEIGHT,CANON_HIT_STRENGHT);
Target bronx(100.0,tft_height-CITY_HEIGHT,CITY_WIDTH,CITY_HEIGHT,CITY_HIT_STRENGTH);

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
  tft.fillScreen(BG_COLOR);
  tft.drawString(4,4,"Starting...",CYAN,1);
  //int meteorNum = random(5,METEOR_MAX);
  meteorNum = 10;
  //tft.drawString(20,80,meteorNum, RED,2);
  //tft.drawChar(20,80,meteorNum,RED,2);
  
  createMeteorArray();
  delay(1500); 
}

/*function: loop()
  ***************************************
  Arduino event loop function which runs continuously on the chip
  while power is delivered.  Functions for balls and paddle are called 
  repeatedly by this function.
*/
void loop() { 
  //int meteorN= metoerNum;
  //int targetN = t;
  for ( int i = 0 ; i < meteorNum; i++){
    for (int j = 0; j < TARGET_TOTAL;j++){
      checkCollision(meteorArray[i],targets[j]);
    }
  }
  drawTargets();
  drawCanonSite();
  drawMeteors();
 // int startX = cur_r  | (cur_g << 5) | (cur_b << 11);
  delay(LOOP_SPEED);
  DEBUG_PRINT(meteorNum);
  DEBUG_PRINT(meteorCount);
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
  tft.fillRect(targetX,targetY+3,6,1,BG_COLOR);
  tft.fillRect(targetX+3,targetY,1,6,BG_COLOR);

  targetX+= xDiff;
  targetY+= yDiff;
  targetX = constrain(targetX,5,tft_width-5);
  targetY = constrain(targetY,5,tft_width-5);
  tft.fillRect(targetX,targetY+3,6,1,BLUE);
  tft.fillRect(targetX+3,targetY,1,6,BLUE);
  if (digitalRead(btnPin0) && millis()-lastPress >400){
    //canonShells[SHELLS_USED].x = targetX;
    //canonShells[SHELLS_USED].y = targetY;
    shootCanon(targetX, targetY);
    tft.fillCircle(targetX,targetY,5,GREEN);
  }
  /*
  DEBUG_PRINT(xRaw);
  DEBUG_PRINT(yRaw);
  DEBUG_PRINT(targetX);
  DEBUG_PRINT(targetY);
  DEBUG_PRINT(xDiff);
  DEBUG_PRINT(yDiff);
  */
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
void createMeteorArray(){
  
  for (int i = 0; i < meteorNum; i++){
     meteorArray[i].x= random(10,tft_width-10);
     meteorArray[i].y=2;
     meteorArray[i].radius=2;
     //tft.fillCircle(meteorArray[i].x, meteorArray[i].y, meteorArray[i].radius,BLUE); 
    }
    //DEBUG_PRINT(meteorNum);
  
}

void drawMeteors(){ 
  DEBUG_PRINT("CALLED drawMeteors");
  if (meteorCount >= meteorNum){meteorCount = 0;}
  for ( int j = 0; j < meteorCount; j++){
      tft.fillCircle(meteorArray[j].x,meteorArray[j].y,meteorArray[j].radius,BG_COLOR);
      DEBUG_PRINT(meteorArray[j].x);
      DEBUG_PRINT(meteorArray[j].y);
      DEBUG_PRINT(meteorArray[j].radius);
      meteorArray[j].y+=meteorDY;
      tft.fillCircle(meteorArray[j].x,meteorArray[j].y,meteorArray[j].radius,BLUE);     
      if (meteorArray[j].y >= tft_height){meteorArray[j]= Target(); }  
    }
  meteorCount++;
}

void drawExplosion(int someX, int someY){
  int explodeTime=4;
  for (int i =0 ; i < explodeTime; i ++){
    tft.fillCircle(someX, someY, i , YELLOW);
  }
  for ( int j=explodeTime; j> 0; j--){
    tft.fillCircle(someX,someY,j , BG_COLOR);
    tft.fillCircle(someX,someY,j , YELLOW);
  }
  
}

void checkCollision (Target enemy, Target ally){
  DEBUG_PRINT("COLLISION CHECKER");
  DEBUG_PRINT(enemy.x);
  DEBUG_PRINT(enemy.y);
  if (enemy.x > ally.x && enemy.x < (ally.x + ally.width) && enemy.y > ally.y){
     drawExplosion(enemy.x, enemy.y); 
  }
}

void shootCanon (int siteX, int siteY){
  float distToX = 0;
  float distToY = 0;
  
  if (SHELLS_USED <=TOTAL_SHELLS ){
    canonShells[SHELLS_USED].x = targetX;
    canonShells[SHELLS_USED].y = targetY;
    for (int i = 0 ; i < TOTAL_SHELLS; i++){
      
    
    
}

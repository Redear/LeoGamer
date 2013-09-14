/*file: Target.cpp

 */
#include <math.h>
#include "Target.h"
//#include <ST7735.h>
/*Default Constructor necessary if you build an array without the variables initialized*/
Target::Target(){};
/*City and Canon Constructor */
Target::Target(float _x,float _y,int _width, int _height,int _strength){
    x= _x;
    y= _y;
    width= _width;
    height= _height;
    strength= _strength;
    killed=0;//0, false
}
/*Meteor Constructor */
Target::Target(float _x, float _y, int _radius, int _strength){
  x = _x;
  y = _y;
  radius = _radius;
  strength = _strength;
  killed=0;// 0 , false
  
}

Target::~Target(){};

float Target::distance(Target &A){
  return sqrt((x-A.x) * (x-A.x) + (y-A.y) * (y-A.y)); //euclidian distance
}
/*
void Target::drawTarget(int _color){
    tft.fillRect(x,y,width,height,_color);
}
*/   

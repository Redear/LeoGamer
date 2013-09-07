/*file: Target.cpp

 */
#include "Target.h"
//#include <ST7735.h>
Target::Target(float _x,float _y,int _width, int _height,int _strength){
    x= _x;
    y= _y;
    strength= _strength;
    width= _width;
    height= _height;
    strength= _strength;
}
Target::~Target(){};
/*
void Target::drawTarget(int _color){
    tft.fillRect(x,y,width,height,_color);
}
*/   

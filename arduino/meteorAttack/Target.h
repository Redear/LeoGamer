#ifndef TARGET_H
#define TARGET_H
/*file: Target.h
 */

class Target {
public:
  Target();
  Target(float _x,float _y,int _width,int _height ,int _strength);
  Target(float _x,float _y,int _radius, int _strength);
  ~Target();
  /* void drawTarget(int _color);*/

public:
  float x,y;
  int strength;
  int width,height;
  int radius;//only for meteors
};

#endif

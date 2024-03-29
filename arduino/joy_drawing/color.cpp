#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Color.h"



Color :: Color(byte r, byte g, byte b) : 
  red(r), green(g), blue(b) { }

void Color::set_color(byte r, byte g, byte b){
  red = r;
  green = g;
  blue = b;
}
int Color::get_color(){
  int cur_r = red ;
  int cur_g = green ;
  int cur_b = blue ;
  my_color = cur_r  | (cur_g << 5) | (cur_b << 11); 
  return my_color;
}
void Color::convert_rgb_to_hsb(float r, float g, float b){
  float max_rgb = max(max(r,g), b);
  float min_rgb = min(min(r,g), b);
  float chroma = max_rgb - min_rgb;
  float h = 0;
  if ( chroma == 0) h = 0;
  else if (max_rgb == r) h = ((g-b) /  chroma) + 0.999; 
  else if (max_rgb == g) h = ((b-r) /  chroma) + 0.666; 
  else if (max_rgb == b) h = ((r-g) /  chroma) + 0.333; 
  float lightness = (r + g + b)/ 3;
   Serial.print(" Hue:  ");
  Serial.print(h);
  Serial.print(", chroma:  ");
  Serial.print(chroma);
  Serial.print(", lightness:  ");
  Serial.println(lightness);
}

void Color::convert_hcl_to_rgb(float h, float c, float l){
  float redf, greenf, bluef;
  if (c == 0 ){
    redf = greenf = bluef = l;
  }
  float temp2;
  if ( l < 0.5) temp2 = l * ( 1 + c);
  else temp2 = l + c - l * c;
  float temp1 = 2.0 * l - temp2;
  float rtemp = h + 0.33333;
  float gtemp = h ;
  float btemp = h - 0.33333;
  if (rtemp > 1 ) rtemp -= 1;
  else if (rtemp < 0 ) rtemp += 1;
  if (gtemp > 1 ) gtemp -= 1;
  else if (gtemp < 0 ) gtemp += 1;
  if (btemp > 1 ) btemp -= 1;
  else if (btemp < 0 ) btemp += 1;
  
  if ( 6.0 * rtemp < 1 ) redf = temp1+ (temp2-temp1) *6.0*rtemp;
  else if ( 2.0 * rtemp < 1 ) redf = temp2;
  else if ( 3.0 * rtemp < 2 ) redf = temp1+ (temp2-temp1) *(0.6666-rtemp)*6.0;
  else redf = temp1;
  
  if ( 6.0 * gtemp < 1 ) greenf = temp1+ (temp2-temp1) *6.0*gtemp;
  else if ( 2.0 * gtemp < 1 ) greenf = temp2;
  else if ( 3.0 * gtemp < 2 ) greenf = temp1+ (temp2-temp1) *(0.6666-gtemp)*6.0;
  else greenf = temp1;
  
  if ( 6.0 * btemp < 1 ) bluef = temp1+ (temp2-temp1) *6.0*btemp;
  else if ( 2.0 * btemp < 1 ) bluef = temp2;
  else if ( 3.0 * btemp < 2 ) bluef = temp1+ (temp2-temp1) *(0.6666-btemp)*6.0;
  else bluef = temp1;
  
  red = (byte) ( 31 * redf);
  green = (byte) ( 63 * greenf);
  blue = (byte) ( 31 * bluef);
}

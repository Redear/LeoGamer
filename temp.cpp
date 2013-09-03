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
  //int cur_y =  1024 - analogRead(2);  
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

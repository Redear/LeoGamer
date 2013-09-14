const int mouseButton = 12;
const int mouseButtonRight = 11;
const int joyX = 3;
const int joyY = 2;
int responseDelay = 10;     // response delay of the mouse, in ms
int maxX = -1;
int minX = 1025;
int maxY = -1;
int minY = 1025;

void setup() {
  // initialize the buttons' inputs:      
  pinMode(mouseButton, INPUT);
  //pinMode(mouseButtonRight, INPUT);  
  // initialize mouse control:
  Mouse.begin();
}

void loop() {

  int clickState = digitalRead(mouseButton);
  //int clickStateRight = digitalRead(mouseButtonRight);
  
  int xVal =  analogRead(joyX);
  int yVal =  analogRead(joyY);
  
  if ( maxX < xVal ) maxX = xVal;
  if ( maxY < yVal ) maxY = yVal;
  if ( minX > xVal ) minX = xVal;
  if ( minY > yVal ) minY = yVal;
  
  xVal = map(xVal, minX, maxX, -10,11);
  yVal = map(yVal, minY, maxY, -10,11);
  
  // if X or Y is non-zero, move:
  if (xVal != 0 || yVal != 0) {
    Mouse.move(xVal, yVal, 0);
  }

  // if the mouse button is pressed:
  if (clickState == HIGH) {
    // if the mouse is not pressed, press it:
    if (!Mouse.isPressed(MOUSE_LEFT)) {
      Mouse.press(MOUSE_LEFT); 
    }
  } 
  // else the mouse button is not pressed:
  else {
    // if the mouse is pressed, release it:
    if (Mouse.isPressed(MOUSE_LEFT)) {
      Mouse.release(MOUSE_LEFT); 
    }
  }
  /* if the mouse button is pressed:
  if (clickStateRight == HIGH) {
    // if the mouse is not pressed, press it:
    if (!Mouse.isPressed(MOUSE_RIGHT)) {
      Mouse.press(MOUSE_RIGHT); 
    }
  } 
  // else the mouse button is not pressed:
  else {
    // if the mouse is pressed, release it:
    if (Mouse.isPressed(MOUSE_RIGHT)) {
      Mouse.release(MOUSE_RIGHT); 
    }
  }
  */
  // a delay so the mouse doesn't move too fast:
  delay(responseDelay);
}

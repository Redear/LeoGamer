/* LucidTronix joy_keyboard 
 * DS1307 based rtc clock
 * For instructions, details and schematic, See:
 * http://www.lucidtronix.com/tutorials/42
 * Uses the RTClib.h library from adafruit
 */
#include <ST7735.h>
#include <SPI.h>
#include <color.h>

#define cs 10   // for MEGAs you probably want this to be pin 53
#define dc 9
#define rst 8  // you can also connect this to the Arduino reset

//ST7735 tft = ST7735(cs, dc, rst);
// for leo:
ST7735 tft = ST7735(SS, dc, rst);
Qq
int btnPin = 12; 

int speakerPin = 6;
int tft_width = 128;
int tft_height = 160;
unsigned long last_press = 0;
unsigned long last_scroll = 0;

Color background_color = Color(0,0,0);
String alphabet = "abcdefghijklmnopqrstuvwxyz ";
int char_index = 15;
char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
int cur_note = 0;

boolean uppercase = false;

int mode = 2;
int num_modes = 3;

void setup(){  
  Serial.begin(9600);
  pinMode(btnPin,INPUT); 
  pinMode(speakerPin, OUTPUT);
  tft.initR();               // initialize a ST7735R chip
  tft.writecommand(ST7735_DISPON); 
  delay(1000);
  tft.fillScreen(BLACK);
  tft.drawString(4, 4, "Starting up...", CYAN,1);
  delay(600);
  tft.fillScreen(background_color.get_color_16bit());  
  Keyboard.begin();
}
void loop(){
  scroll_keyboard();
}
void scroll_keyboard(){
  int joy_x = analogRead(0);
  int joy_y = analogRead(1);
  int y_dif = map(joy_y, 0,1024,1,-2);
  int index_dif = map(joy_x, 0,1024,-3,4);  
  if ((y_dif!=0 || index_dif != 0)  && millis()-last_scroll > 320 ){
    
    if ( y_dif == -1){
     if (uppercase) alphabet.toLowerCase();
     else alphabet.toUpperCase();
     uppercase = !uppercase;
    } 
    tft.fillRect(1,29,125,33,background_color.get_color_16bit());
    char_index += index_dif; 
    if ( char_index < 0 ) char_index = alphabet.length() + char_index;
    if ( char_index >= alphabet.length() ) char_index = char_index-alphabet.length();
    int xpos = 2;
    for (int i = char_index-4 ; i <= char_index+4; i++){
      int cur_index = i;
      if ( i < 0 ) cur_index = alphabet.length() + i;
      if ( i >= alphabet.length() ) cur_index = i-alphabet.length();
      if (i == char_index){
        tft.drawChar(xpos-2, 30,alphabet.charAt(cur_index), YELLOW,3);
        tft.drawRect(xpos-5,28,20,30, WHITE);
        xpos += 5;     
      }
      else if ( abs(i - char_index) < 3){
        tft.drawChar(xpos, 34,alphabet.charAt(cur_index), CYAN,2);
        xpos+=3;
      }
      else tft.drawChar(xpos, 40,alphabet.charAt(cur_index), CYAN,1);
      xpos += 12;
    }
    last_scroll = millis();
  }
  if ( digitalRead(btnPin) && millis()-last_press > 500 ){
    last_press = millis();
    Keyboard.print(alphabet.charAt(char_index));
  }
}
void sensor_text(){
}

char* int2chars(int val){
  String s = String(val);
  char cx[6];
  cx[0] = s.charAt(0);
  cx[1] = s.charAt(1);
  cx[2] = s.charAt(2);
  cx[3] = s.charAt(3);
  cx[4] = s.charAt(4);
  cx[5] = 0;
  return cx;
}

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}
void playNote(char note, int duration) {
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

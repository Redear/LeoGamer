/* LucidTronix Alarm Clock 
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

int btnPin0 = 10; 
int btnPin1 = 11;

int speakerPin = 6;
int potPin = 0;
int cur_sec = 0;
int cur_min = 0;
int millisecond_six = 0;
int marble_x = 60;
int marble_y = 76;
int marble_radius = 6;
int tft_width = 128;
int tft_height = 160;
unsigned long last_sec = 0;
unsigned long last_press = 0;
unsigned long last_point_increment = 0;
const int num_holes =  10;
int hole_x[num_holes] ;
int hole_y[num_holes] ;
int hole_radius[num_holes] ;
int hole_type[num_holes];
int goal_x, goal_y, goal_radius;
int points = 0;
int lives = 5;
Color background_color = Color(0,0,0);
int old_val = 0;
char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
int cur_note = 0;
RTC_DS1307 RTC;

DateTime cur_time, alarm_time;
boolean alarm_on = true;
boolean set_minute = false;

// Store our compass as a variable.
HMC5883L compass;
// Record any errors that may occur in the compass.
int error = 0;

int mode = 2;
int num_modes = 3;

void setup(){  
  Serial.begin(9600);
  pinMode(btnPin1,INPUT);
  pinMode(btnPin0,INPUT); 
  pinMode(speakerPin, OUTPUT);
  tft.initR();               // initialize a ST7735R chip
  tft.writecommand(ST7735_DISPON); 
  delay(1000);
  tft.fillScreen(BLACK);
  tft.drawString(4, 4, "Starting up...", CYAN,1);
  delay(600);
  tft.fillScreen(background_color.get_color_16bit());  
}
void loop(){
  sensor_text();
  if (digitalRead(btnPin0) && millis() - last_press > 400) {
    mode = ++mode % num_modes; 
    last_press = millis();
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

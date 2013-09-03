/*
File lucidPlanetCommand.ino
***********************************************************
a clone of the classic game Missile Command for the arduino
Leonardo architecture.  This game uses a single button to
control multiple cannons that are tasked with protecting the
planet Choctaw from being destroyed by the missile onslaught of
the evil Emporer Georgenon Shrubins.

URL FOR TUTORIAL:: XXX
This file uses the adafruit library ST7735 which can be
found at this url: XXXXXXXXXXXXXXXXXXXXXX

joystick read lines:
digitalRead(12)
int cur_x =  analogRead(0);
int cur_y =  1024 - analogRead(2);  
int cur_y =  1024 - analogRead(1);
*/

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

// sc ----> SS wire difference SLAVE SELECT
ST7735 tft = ST7735(SS, dc, rst);

const int tft_width = 128;
const int tft_height = 160;

/*

*/
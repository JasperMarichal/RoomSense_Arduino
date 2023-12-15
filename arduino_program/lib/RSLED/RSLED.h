/*
RoomSense RGB-LED Library

Using LED module KY-016
*/

#ifndef RSLED_h
#define RSLED_h

#include <Arduino.h>

#ifndef RSLED_R
#define RSLED_R 11
#endif
#ifndef RSLED_G
#define RSLED_G 12
#endif
#ifndef RSLED_B
#define RSLED_B 13
#endif

void initLED(); // call from setup

void updateLED(); // call from loop

/*

ledcmd changes the colors based on an led command string,
the commands are formatted as such:

[LED][R | r][G | g][B | b]

The command optionally starts with "LED",
then the color is specified.

The red green and blue values are optional, if one is not given
it keeps its current state. Otherwise capital letters means to enable,
and lowercase means to disable the corresponding color.

*/
void ledcmd(String cmd);

#endif

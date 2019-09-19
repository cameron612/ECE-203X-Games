#include "speech.h"
#include "graphics.h"
#include "globals.h"
#include "hardware.h"

/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();

/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
static void draw_speech_line(const char* line, int which);

/**
 * Delay until it is time to scroll.
 */
static void speech_bubble_wait();

void draw_speech_bubble()
{
    uLCD.filled_rectangle(0, 65, 130, 120, LGREY);
}

void erase_speech_bubble()
{
    //draw_game(true);
}

void draw_speech_line(const char* line, int which)
{
    if(which == TOP){
        uLCD.text_string((char*)line, 0, 9, FONT_7X8, WHITE);
    }
    if(which == BOTTOM){
        uLCD.text_string((char*)line, 0, 12, FONT_7X8, WHITE);
    }
}

void speech_bubble_wait()
{
    wait(3.6);
}

void speech(const char* line1, const char* line2)
{
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_line(line2, BOTTOM);
    speech_bubble_wait();
    erase_speech_bubble();
}

void long_speech(const char* lines[], int n)
{
}

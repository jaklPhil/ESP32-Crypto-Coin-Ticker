#pragma once
#include <Arduino.h>

#define LEDMATRIX_SEGMENTS 12



#define LEDMATRIX_CS_PIN 15




void drawSprite( byte* sprite, int x, int y, int width, int height );
void drawString(char* text, int len, int x, int y );
void scrollString(char* text, int speed);
void pushMatrixCoinData(String tmpName, String tmpText);
void drawBufferString(char *text, int len, int x, int y, int buffer);
void setupMDisplay(int brightness);
void updateDisplay();
void scrollMatrix();
void DisplayRun();

void SetupMatrixEEPROM();
void printMSG(String msg, int x);
void MatrixDisplayCoinData();
String createCoinString(String price, String day_change, String hr_change);
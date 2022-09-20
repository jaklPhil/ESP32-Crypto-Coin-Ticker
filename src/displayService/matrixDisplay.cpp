#include <Arduino.h>
#include "displayService/matrixDisplay.h"
#include "displayService/font.h"
#include <LEDMatrixDriver.hpp>
#include "CoinAPI/coinTicker.h"
#include "CoinAPI/model.h"


//cryptoCoins cryptoModel defined via model.h

// The LEDMatrixDriver class instance
LEDMatrixDriver lmd(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN);
const int LEDMATRIX_WIDTH = LEDMATRIX_SEGMENTS * 8;

//#define L2R //Left to right scroll
#define R2L		//Right to left scroll

//define for Serial debug
//#define DebugMode

int x = 0, y = 0; // start top left

int nameLen=0,  len =0;
 int selCoin = 0;
 char name[5];
  char text[45];


void setupMDisplay(int brightness)
{
  // init the display
  lmd.setEnabled(true);
  lmd.setIntensity(brightness); // 0 = low, 10 = high
}

void updateDisplay()
{
  lmd.display();
}

void scrollString(char *text, int speed)
{
  // Draw the text to the current position
   len = strlen(text);
  drawString(text, len, x, 0); // In case you wonder why we don't have to call lmd.clear() in every loop: The font has a opaque (black) background...
  // Toggle display of the new framebuffer
  lmd.display();


#ifdef L2R
  //For left to right scroll
  if (++x > LEDMATRIX_WIDTH)
  {
    x = -len * 8;
  }
#endif
#ifdef R2L
  //For right to left scroll
  if (--x < len * -8)
  {
    x = LEDMATRIX_WIDTH;
    if (selCoin >= cryptoModel.numOfCoins -1){
      //rest coin loop
      selCoin = 0;
      
    } else {
      // next coin
      selCoin++;
    }
    
  }
#endif
}

void scrollMatrix()
//draw Currently loaded coin
{
  // Draw the text to the current position
  if(name == NULL){
     Serial.println("null !!!!!!!!!!");
    return;
  }
 int nameLen = strlen(name);
  int len = strlen(text);
 lmd.clear();
  drawString(text, len ,  x -10 , 0);
  drawString(name, nameLen, 0, 0);
  //drawBufferString(text, len, nameLen + x, 0,nameLen + 1);
  // In case you wonder why we don't have to call lmd.clear() in every loop: The font has a opaque (black) background...
 
  // Toggle display of the new framebuffer
  lmd.display();

#ifdef L2R
  //For left to right scroll
  if (++x > LEDMATRIX_WIDTH)
  {
    x = -len * 8;
  }
#endif

#ifdef R2L

  #ifdef DebugMode
    String msg, msg2;
    msg2 = "scroll val: " + String(x) + " / " + String((len - nameLen ) * -8 );
  
    //Serial.println(msg2);
  #endif

  //For right to left scroll
  if (--x < (len - nameLen -1 ) * -8 )
  {
    x = LEDMATRIX_WIDTH + nameLen * 8;

    if (selCoin >= cryptoModel.numOfCoins -1) {
      //reset coin loop
      selCoin = 0;
    } else {
      //display next coin
      selCoin++;
    }
    MatrixDisplayCoinData();

  #ifdef DebugMode
    msg = "Coin: " + String(selCoin + 1 )  + " / " + cryptoModel.numOfCoins + " - " + cryptoModel.coins[selCoin].name;
    Serial.println(msg);
    
  #endif
  }
#endif


}
void pushMatrixCoinData(String tmpName, String tmpText)
{

  tmpName.toCharArray(name, sizeof(name));
  tmpText.toCharArray(text, sizeof(text));
  #ifdef DebugMode
    Serial.print("displayCoin ");
    Serial.println(tmpText);
    Serial.println(text);
  #endif
}

void drawSprite(byte *sprite, int x, int y, int width, int height)
{
  // The mask is used to get the column bit from the sprite row
  byte mask = B10000000;

  for (int iy = 0; iy < height; iy++)
  {
    for (int ix = 0; ix < width; ix++)
    {
      lmd.setPixel(x + ix, y + iy, (bool)(sprite[iy] & mask));

      // shift the mask by one pixel to the right
      mask = mask >> 1;
    }

    // reset column mask
    mask = B10000000;
  }
}

/**
 * This function draws a string of the given length to the given position.
 */
void drawString(char *text, int len, int x, int y)
{
  for (int idx = 0; idx < len; idx++)
  {
    int c = text[idx] - 32;
    
    // stop if char is outside visible area
    if (x + idx * 8 > LEDMATRIX_WIDTH)
      return;

    // only draw if char is visible
    if (8 + x + idx * 8 > 0)
      drawSprite(font[c], x + idx * 8, y, 8, 8);
  }
}


void drawCoin(char *text, int len, int x, int y)
{
  for (int idx = 0; idx < len; idx++)
  {
    int c = text[idx] - 32;

     Serial.print(text[idx]);
     Serial.print(" Buchstaben NR: ");
     Serial.println(c);
    
    // stop if char is outside visible area
    if (x + idx * 8 > LEDMATRIX_WIDTH)
      return;

    // only draw if char is visible
    if (8 + x + idx * 8 > 0)
      drawSprite(font[c], x + idx * 8, y, 8, 8);
  }
}

void MatrixDisplayCoinData(){
  String tmpCoinName;
  String tmpCoinStr;

  //Coins
  cryptoCoin* tmpCoin;
  
  //Load data
  tmpCoin = cryptoModel.getCoin(selCoin);

  //create Display strings
  tmpCoinName =  tmpCoin->name; 
  tmpCoinStr = createCoinString(tmpCoin->price, tmpCoin->day_percent_change ,tmpCoin->hr_percent_change); 

  pushMatrixCoinData(tmpCoinName, tmpCoinStr);

}

String createCoinString(String price, String day_change, String hr_change){
    String tmpStr;
    tmpStr = price + "$ #" + day_change + "% @" + hr_change + "%";
    return tmpStr;
}

void DisplayRun(){
    lmd.display();
}
void printMSG(String msg, int x){
    char ipText[30];
    msg.toUpperCase();
    msg.toCharArray(ipText, sizeof(ipText));
    int ipLen = strlen(ipText);
    drawString(ipText,ipLen,x,0);
    DisplayRun();
}
//not used for normal string
void drawBufferString(char *text, int len, int x, int y, int buffer)
{
  for (int idx = 0; idx < len; idx++)
  {
    int c = text[idx] - 32;
    
    // stop if char is outside visible area
    if ( x + idx * 8 > LEDMATRIX_WIDTH)
      return;

    // only draw if char is visible
    if (8 + x + idx * 8 > 0)
      drawSprite(font[c], buffer*8 +  x + idx * 8, y, 8, 8);
  }
}
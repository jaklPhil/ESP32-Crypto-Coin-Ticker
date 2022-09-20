

#include "espService/setupWifi.h"

//#include "CoinAPI/coinTicker.h"
#include "espService/taskService.h"
#include "espService/dataService.h"
#include <Arduino.h>
#include "espService/dataService.h"
#include "displayService/matrixDisplay.h"

#include "WebServer/inputService.h"
void setup() {
  Serial.begin(115200);

  //MAtrix led 
   //setupMDisplay(1); //0-10 low-hogh

    setupEEprom();
    delay(1000);
    
    //clearCoins();
    SetupMatrixEEPROM();
    //standard coins
    //saveCoin("BTC");

    // saveCoin("ETH");
    // saveCoin("DOGE");


  //setup WIFI connection, Wait until succeed connecting
    setupWIFI();

//setupConfigService();
    //startServer();
    setupServer();

  createTasks();
  
}

void loop() {
  //getData();
  //delay(1000);
  //runConfigService();
}
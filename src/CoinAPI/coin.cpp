#include <Arduino.h>
#include "CoinAPI/coin.h"
#include "CoinAPI/model.h"
#include "espService/dataService.h"

cryptoCoins cryptoModel; 
//coinNames[MAXnumOfCoins];
cryptoCoins::cryptoCoins(){
    //load coins from EEprom
    loadCoins();
  }

void cryptoCoins::loadCoins(){
        Serial.println("load coin ticker:    ");
        //load Number of Stored Coins
        cryptoModel.numOfCoins	= getCoinNum();
        //Create Cryptocoins
        for(int x=0; x < cryptoModel.numOfCoins; x++) {
            String tmpName = "";
            tmpName = getCoinName(x);
            //this->coinNames[x] = tmpName;
            cryptoModel.coins[x] =  cryptoCoin{tmpName,"","",""};
            Serial.println("load coin ticker:    ");
        }
}

cryptoCoins cryptoCoins::getCoinTickerData(){
  cryptoCoins tmpCoins;
  tmpCoins = getCryptocoins();
 
  Serial.print("Get coin ticker! name...");
  Serial.println(tmpCoins.coins[0].name);
  return tmpCoins;
}

cryptoCoin* cryptoCoins::getCoin(int coinIndex){
    return &cryptoModel.coins[coinIndex];
    //return &this->coins[coinIndex];
}

void  updateData(){
  cryptoModel.getCoinData();
}



cryptoCoins getCryptocoins(){
  return cryptoModel;
}
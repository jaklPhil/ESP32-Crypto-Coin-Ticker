#include <EEPROM.h>
#include <Arduino.h>
#include "espService/dataService.h"
#include "CoinAPI/coinTicker.h"
#include "displayService/matrixDisplay.h"
#include "espService/taskService.h"

const uint8_t maxCoins = 10;
//Space before coins
uint8_t CoinStart = 4;
uint8_t strLenght = 4;

//local Variables
uint8_t CoinsNr;
uint8_t martixBrightness;
uint8_t martixSpeed;
String EEcoinNames[maxCoins];
boolean coinExists;

void setupEEprom(){
 
EEPROM.begin(512);
//EEPROM.write(0, 0); 
       // EEPROM.commit();

}

// String* coinNames()
// {
//     loadCoins();
//     return EEcoinNames;
// }

void loadCoins(){
   //load brightness etc...
    String tmpName;
    CoinsNr = uint8_t(EEPROM.read(0));

    
    for(int coin=0; coin<CoinsNr; coin++){
        tmpName = "";
        for(int i=0; i<strLenght; i++){
            char value= char(EEPROM.read(CoinStart + i+(coin*strLenght)));
            if(String(value) != " "){
                 tmpName += value;
            }
           
        }
        
        EEcoinNames[coin] = tmpName;
        Serial.print("load coin............... ");
        Serial.print(coin);
        Serial.print(" - ");
        Serial.println(EEcoinNames[coin]);

    }
}


void writeCoin(String coinName){
    

            for(int i=0; i<strLenght; i++){
                    int adress = CoinStart + i+(CoinsNr *strLenght);
                    Serial.print ("write to adress ");
                    Serial.print (adress);
                    Serial.print ("  ");
                    
                    if (coinName.charAt(i) == NULL){
                        char tmpccc = ' ';
                        EEPROM.write(adress, tmpccc); 
                    } else{
                            Serial.println (coinName.charAt(i));
                    EEPROM.write(adress, coinName.charAt(i));  
                    } 
                }
            CoinsNr ++;
            EEPROM.commit();
            EEPROM.write(0, CoinsNr);

                
            
            EEPROM.commit();
        
}
boolean coinExistsEEPROM(String coinName){
    for (int oldCoin = 0; oldCoin < CoinsNr; oldCoin++){
        
        if( (EEcoinNames[oldCoin] == coinName)){
            return true;
        }
    } 
    return false;
}
void saveCoin(String coinName){
    loadCoins();
    
    //catch double coin name
    
    coinExists = coinExistsEEPROM(coinName);

    if (CoinsNr == 0){ 
        coinExists = false;
    }
    if(!coinExists){
        //Create coin
        writeCoin(coinName);
        //update Coin Data
        requestCoinData();
     }
}
uint8_t getCoinNum(){
    loadCoins();
    return CoinsNr;
}
String getCoinName(int num){
    loadCoins();
    return EEcoinNames[num];
}
void setBrightness(int brightness){
    //Set brighness
    setupMDisplay(brightness);
    //save
    EEPROM.write(1, uint8_t(brightness));
    EEPROM.commit();
}

uint8_t getBrightness(){
    martixBrightness = uint8_t(EEPROM.read(1));
    Serial.print("Read EEPRom brightness: ");
    Serial.println(martixBrightness);

    return martixBrightness;
}
void SetupMatrixEEPROM(){
    //load Brightness
    setupMDisplay(getBrightness());
    //LoadSpeed
    setSpeed(getSpeed());
    
}

uint8_t getSpeed(){
    martixSpeed = uint8_t(EEPROM.read(2));
    if (martixSpeed < 1){
        martixSpeed = 50;
    }
    return martixSpeed;
}


void setSpeed(int speed){
    
    if (speed >= MIN_SPEED || speed <= MAX_SPEED){
     SetMatrixSpeed(speed);
     Serial.print("Speed set:   ");

     Serial.println(speed);

    EEPROM.write(2, speed);
    EEPROM.commit();
  }  
}


void deleteCoinData(String coinDel){

    String tmpCoinNames[maxCoins];
    int newCoin= 0;
    for (int oldCoin = 0; oldCoin < CoinsNr; oldCoin++){
         Serial.print("copin NR ");
        Serial.println(EEcoinNames[oldCoin]);
        if( !(EEcoinNames[oldCoin] == coinDel)){
            Serial.print("Save old");
        Serial.print(oldCoin);
        Serial.print(" - ");
        Serial.println(EEcoinNames[oldCoin]);
            Serial.print("copin NR ");
        Serial.println(oldCoin);
        
            tmpCoinNames[newCoin] = EEcoinNames[oldCoin];
            newCoin++;
        }
    } 
    //Save coins
    Serial.print("newCoin ");
    Serial.println(newCoin);

    Serial.print("Old coin nr ");
    Serial.println(CoinsNr);
    CoinsNr = newCoin;
    for (int tmpCoin = 0; tmpCoin < CoinsNr; tmpCoin++){
    EEcoinNames[tmpCoin] = tmpCoinNames[tmpCoin];
    }
    EEPROM.write(0, CoinsNr);
    EEPROM.commit();


    for (int CoinNR = 0; CoinNR < CoinsNr; CoinNR++){
        
        //Write all coins
        Serial.print("Write new.............. ");
        Serial.print(CoinNR);
        Serial.print(" - ");
        Serial.println(tmpCoinNames[CoinNR]);
        writeCoinToEEPROM(tmpCoinNames[CoinNR], CoinNR);
    } 


    //Refresch Ticker data
    requestCoinData();
}  


void writeCoinToEEPROM(String EEcoinName, int coin){
        for(int i=0; i<strLenght; i++){
        int adress = CoinStart + i+(coin *strLenght);
        Serial.print ("write to adress start 5 ");
        Serial.print (adress);
        Serial.print ("  ");
        
        if (EEcoinName.charAt(i) == NULL){
            char tmpccc = ' ';
            EEPROM.write(adress, tmpccc); 
        } else{
                Serial.println (EEcoinName.charAt(i));
        EEPROM.write(adress, EEcoinName.charAt(i));  
        } 
    }
}


void clearCoins(){
    for (int i = 0; i < 512; i++) {
     EEPROM.write(i, 0);
     }
     //
    EEPROM.commit();
    //saveCoin("BTC");
    //Refresch Ticker data
    requestCoinData();
}

    


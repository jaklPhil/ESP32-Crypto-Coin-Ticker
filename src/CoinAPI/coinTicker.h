#pragma once
#include <ArduinoJson.h>
#include "CoinAPI/coin.h"

//Setup Ticker Parameters
const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 100; //Set size of the Json object
const String cryptoCompare = "https://min-api.cryptocompare.com/data/pricemultifull?fsyms=";
const String exchange = "Coinbase";



//Global coin number from EEPROM
extern uint8_t numOfCoins;



cryptoCoin* getCoin(int coinIndex);

cryptoCoins getCoinTickerData();
void requestCoinData();
//void printCoinData():
void printCoinData();
void MatrixCoinData();

void ParseJSONtoCoin(String payload);
String formatCoinVal(String val);
String createRequest();
 String formatCoinPrice(String price);
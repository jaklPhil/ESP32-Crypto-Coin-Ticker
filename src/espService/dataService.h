#pragma once
#include <Arduino.h>
#include "CoinAPI/coin.h"

void setupEEprom();
void clearCoins();
void loadCoins();
void saveCoin(String coinName);
//String* coinNames();
uint8_t getCoinNum();
String getCoinName(int num);

void setSpeed(int speed);
uint8_t getSpeed();
void setBrightness(int brightness);
uint8_t getBrightness();

void deleteCoinData(String coin);
void writeCoinToEEPROM(String EEcoinName, int coin);
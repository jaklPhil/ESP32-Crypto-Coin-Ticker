#pragma once
#include <Arduino.h>

#define MAXnumOfCoins 10



struct cryptoCoin{
	String name;
	String price;
	String hr_percent_change;
	String day_percent_change;
};

class cryptoCoins{
		


  public:
		cryptoCoin coins[MAXnumOfCoins];
  	uint8_t numOfCoins;
	cryptoCoins();
	void getCoinData();
	void loadCoins();
	cryptoCoin* getCoin(int coinIndex);
	cryptoCoins getCoinTickerData();
};

//cryptoCoins cryptoModel; 

void updateData();
cryptoCoins getCryptocoins();
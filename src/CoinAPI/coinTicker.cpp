#include <Arduino.h>
#include "CoinAPI/coinTicker.h"
#include "CoinAPI/coin.h"
#include "espService/setupWifi.h"

#include "espService/dataService.h"
#include <HTTPClient.h>
#include "displayService/matrixDisplay.h"

#include "CoinAPI/model.h"

// class functions
void cryptoCoins::getCoinData() {
    

    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    
    while(1){

     requestCoinData();
      //delay task
      vTaskDelayUntil(&xLastWakeTime ,60 * 1000/ portTICK_PERIOD_MS);
    }
   
  }
  void requestCoinData(){
    //refresh/create  coins and brightness
      cryptoCoins();
      
      HTTPClient http;
      http.begin(createRequest());
      int httpCode = http.GET();
      //process data    
      if (httpCode > 0) { //Check for the returning code
         
                String payload = http.getString();

               Serial.println(httpCode);
               //Serial.println(payload);

               //convert Json to local coin Class
               ParseJSONtoCoin(payload);
               Serial.println("Successful loaded!");
               //Push coin Data to Matrix
               MatrixDisplayCoinData();
        }

      http.end(); //Free the resources

      
  }

  void ParseJSONtoCoin(String payload){
    // Parse JSON object
    DynamicJsonDocument doc(6500);
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      //json Error
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
             
    for(int x=0; x < MAXnumOfCoins; x++) 	{

    String tmpCoinName = cryptoModel.coins[x].name;

            String preis = doc["DISPLAY"][tmpCoinName]["EUR"]["PRICE"].as<String>();
           

            String dayProzent = doc["DISPLAY"][tmpCoinName]["EUR"]["CHANGEPCT24HOUR"].as<String>();
            String hrProzent = doc["DISPLAY"][tmpCoinName]["EUR"]["CHANGEPCTHOUR"].as<String>();
            

            //cryptoModel.coins[x].name = coinNames[x];
            cryptoModel.coins[x].price = formatCoinPrice(preis);
            //format value with + for arrow
            cryptoModel.coins[x].day_percent_change  =  formatCoinVal(dayProzent);
            cryptoModel.coins[x].hr_percent_change  = formatCoinVal(hrProzent);
           
          }
  }
  
  String formatCoinVal(String val){
      String tmpStr = "";
      if (val.toFloat() > 0){
        //Serial.println("percent over 0 !!!!!!!!!!");
        tmpStr = "+";
      }
      val.replace(".",",");
      return tmpStr + val;
  } 
   String formatCoinPrice(String Tprice){
      String tmpStr = "";

      //delete unused char
        Tprice.replace("€ ","");
       Tprice.replace(",","");
       Tprice.replace(".",",");
       
      //  Serial.println("formatprice: ");
      //  Serial.print("string: ");
      // Serial.println(Tprice);
      //  Serial.print("float: ");
      // Serial.println(Tprice.toFloat());
    
      return Tprice;
  } 
  String createRequest(){
    String request = cryptoCompare;
      
      for(int x=0; x < MAXnumOfCoins; x++){
        request += cryptoModel.coins[x].name + ",";
      } 

      request += "&tsyms=EUR";
      request += "&e=";
      request += exchange;
      Serial.println(request);

      return request;
  }




//internal function
cryptoCoins crypto; 
struct cryptoCoin* tmpCoin;
String tmpStr;

void printCoinData(){
  //print coin to oled

  //String tmpStr;
  char msg[20];
  

  for(int x=0; x < 1; x++) 	{
    int line = 2;
    tmpCoin = cryptoModel.getCoin(x);
    //String construction
    tmpStr = tmpCoin->name;
    tmpStr += ": ";
    tmpStr += tmpCoin->price;

    tmpStr.toCharArray(msg, sizeof(msg));  
   
    //Serial.println(msg);
    line++;

    tmpStr = "Euro 24h: ";
    tmpStr += tmpCoin->day_percent_change;
    tmpStr += "%";

    tmpStr.toCharArray(msg, sizeof(msg));  
  
    //Serial.println(msg);
    line++;

    tmpStr = " 1h:  ";
    tmpStr += tmpCoin->hr_percent_change;
    tmpStr += "%";

    tmpStr.toCharArray(msg, sizeof(msg));  
   
    //Serial.println(msg);
    line++;


  }
  
  
}

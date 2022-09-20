#include <Arduino.h>
#include "CoinAPI/extTest.h"

void callTest(void *parameter){
    while(1){
      Serial.print("test: function runnung");
      
            
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
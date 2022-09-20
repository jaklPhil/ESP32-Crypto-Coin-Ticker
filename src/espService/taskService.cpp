#include <Arduino.h>
#include "CoinAPI/coin.h"
#include "espService/taskService.h"

//#include "CoinAPI/coinTicker.h"

#include "CoinAPI/coinTicker.h"
#include "CoinAPI/extTest.h"
#include "displayService/matrixDisplay.h"

#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif
int counter = 0;
// in tick ms
int scrollSpeed = 70;

void createTasks()
{

  xTaskCreatePinnedToCore( // Use xTaskCreate() in vanilla FreeRTOS
      toggleCount,         // Function to be called
      "Toggle counter",    // Name of task
      10024,               // Stack size (bytes in ESP32, words in FreeRTOS)
      NULL,                // Parameter to pass to function
      1,                   // Task priority (0 to configMAX_PRIORITIES - 1)
      NULL,                // Task handle
      app_cpu);            // Run on one core for demo purposes (ESP32 only)

  xTaskCreatePinnedToCore( // Use xTaskCreate() in vanilla FreeRTOS
      callGetData,         // Function to be called
      "Load data",         // Name of task
      10024,               // Stack size (bytes in ESP32, words in FreeRTOS)
      NULL,                // Parameter to pass to function
      0,                   // Task priority (0 to configMAX_PRIORITIES - 1)
      NULL,                // Task handle
      app_cpu);            // Run on one core for demo purposes (ESP32 only)

  xTaskCreatePinnedToCore(runMatrix, "Run LED Matrix", 4024, NULL, 0, NULL, app_cpu);
  // xTaskCreatePinnedToCore(runServer,  "Run Config service", 4024, NULL, 2,  NULL, app_cpu);

  vTaskDelete(NULL);
  vTaskStartScheduler();
}

void toggleCount(void *parameter)
{
  while (1)
  {
    printCoinData();
    // MatrixCoinData();
    // MatrixDisplayCoinData();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void runMatrix(void *parameter)
{

  while (1)
  {

    scrollMatrix();
    vTaskDelay(scrollSpeed / portTICK_PERIOD_MS);
  }
}

void SetMatrixSpeed(int speed)
{
  // 1-100
  Serial.print("speed");
  Serial.println(speed);
  scrollSpeed = (MAX_SPEED / speed) * SCROLL_SPEED;
}

void callGetData(void *parameter)
{
  while (1)
  {
    updateData();
  }
}

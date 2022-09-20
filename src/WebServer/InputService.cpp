#include <Arduino.h>


#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include "WebServer/InputService.h"
#include "espService/dataService.h"
#include "espService/taskService.h"
AsyncWebServer server(80);

const char* ssid = "Moechtebert";
const char* password = "moechtebernd";

const char* PARAM_COIN = "inputCoin";
const char* PARAM_DELETE_COIN = "deleteCoin";
const char* PARAM_BRIGHT = "inputBright";
const char* PARAM_SPEED = "inputSpeed";
const char* PARAM_RESET = "inputReset";

//HTML
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Coin Ticker</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Times New Roman; display: inline-block; text-align: center;}
    h1 {font-size: 3.0rem; color: #FF0000;}
    form {font-size: 1.3rem; color: #000000;}
    input {  font-weight: bold}
    form.input {border: 1px solid #ddd; padding: 12px;}
   table {border-collapse: collapse; margin-left:auto; margin-right:auto; }
            th { padding: 12px; background-color: #0043af; color: white; }
            tr { border: 1px solid #ddd; padding: 12px; }
            tr:hover { background-color: #bcbcbc; }
            td { border: none; padding: 12px; }
            .sensor { color:white; font-weight: bold; background-color: #bcbcbc; padding: 1px; }
            .reset {  color:rgb(184, 7, 7); }
  </style>
  <script>
    function submitMessage() {
      alert("Speichern ?");
      setTimeout(function(){ document.location.reload(false); }, 500);   
    }
  </script></head><body>
 <h1>JP Coin Ticker</h1>
  <form class="input" action="/get" target="hidden-form">
      Coin anlegen: <br><input type="text" name="inputCoin">
      <input type="submit" value="Coin Speichern" onclick="submitMessage()">
   
    </form><br>
    <form class="input" action="/get" target="hidden-form">
      Coin entfernen: <br><input type="text" name="deleteCoin">
      <input type="submit" value="Coin entfernen" onclick="submitMessage()">
   
    </form><br>
  <form>
        </form><br>
        <form class="input">
            <table>
                <tr>Aktuell hinterlegt:</th>
                %inputTable%        
        </table>           
  </form><br> 

    <form class="input" action="/get" target="hidden-form">
        Helligkeit (%inputBright%): <br><input type="text" name="inputBright">
        <input type="submit" value="Speichern" onclick="submitMessage()">
      </form><br>
      <form class="input" action="/get" target="hidden-form">
        Geschwindigkeit (%inputSpeed%): <br><input type="text" name="inputSpeed">
        <input type="submit" value="Speichern" onclick="submitMessage()">
      </form><br>
      <form class="input" action="/get" target="hidden-form">
        <input style="display:none" type="text" name="inputReset">
        <input class="reset" type="submit" value="Reset all" onclick="submitMessage()">
      </form><br>
  <iframe style="display:none" name="hidden-form"></iframe>
</body></html>)rawliteral";


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String createTableHTML(){
  String tmpString = "";
  int maxRow =  getCoinNum();
            

            //load coins to table
            for (int tableRow = 0; tableRow < maxRow; tableRow++)
            {
              //Table Row
              tmpString += "<tr><td>"; 
              tmpString += getCoinName(tableRow);
              tmpString += "</td><td>";
            }
  return tmpString;
}
// Replaces placeholder with stored values
String processor(const String& var){
  //Serial.println(var);
  
  if(var == "inputTable"){
    return createTableHTML();
  }
  if(var == "inputBright"){
    

    
      return String(getBrightness())+ " / " + MAX_BRIGHT;
    
    
  }
  if(var == "inputSpeed"){ //use for table
    
      return String(getSpeed()) + " / " + MAX_SPEED;
    
  }
  if(var == "inputReset"){ //use for table
    
    //return; //readFile(SPIFFS, "/inputInt.txt");
  }
  return String();
}

void setupServer() {

  //conected to wifi
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());


  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/get?inputString=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET inputString value on <ESP_IP>/get?inputString=<inputMessage>
    if (request->hasParam(PARAM_COIN)) {

      inputMessage = request->getParam(PARAM_COIN)->value();
      //Write input Coin
      inputMessage.toUpperCase();
      saveCoin(inputMessage);
    }else 
    if (request->hasParam(PARAM_SPEED)) {
      inputMessage = request->getParam(PARAM_SPEED)->value();
      //set MAtrixSpeed
      setSpeed(inputMessage.toInt());
    }else 
    if (request->hasParam(PARAM_BRIGHT)) {
      inputMessage = request->getParam(PARAM_BRIGHT)->value();
      //set brightness
      setBrightness(inputMessage.toInt());
    }else 
    if (request->hasParam(PARAM_DELETE_COIN)) {
      inputMessage = request->getParam(PARAM_DELETE_COIN)->value();
      //Write input 'Messege
      inputMessage.toUpperCase();
      deleteCoinData(inputMessage);
      //writeFile(SPIFFS, "/inputString.txt", inputMessage.c_str());
    }else
    if (request->hasParam(PARAM_RESET)) {
      
      clearCoins();
      Serial.println("cleard.........................");
      //writeFile(SPIFFS, "/inputString.txt", inputMessage.c_str());
    }else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/text", inputMessage);
  });

  server.onNotFound(notFound);
  server.begin();

  
}


#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <HTTPClient.h>
#include <ESPAsyncWebServer.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

AsyncWebServer server(80);
// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid ="Inteli-COLLEGE" ;
const char* password = " QazWsx@123";
const char* PARAM_INPUT_1 = "input1";
// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action=" /get">
    input1: <input type="text" name="input1">
    <input type="submit" value="Submit">
  </form><br>
</body></html>)rawliteral";
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!" );
    return;
  }
  Serial.println();
  Serial.print("IP Address: " );
  Serial.println(WiFi.localIP());
  // Send web page with input fields to client
  server.on(" /", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on(" /get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field ("
                                     + inputParam + " ) with value: " + inputMessage +
                                     " <br><a href=\" /\" >Return to Home Page</a>" );
  });
  server.onNotFound(notFound);
  server.begin();
}
void loop() {
  Serial.println(WiFi.localIP());
  delay(5000);
}
#include<ESP8266WiFi.h>
#include<ESPAsyncTCP.h>
#include<ESPAsyncWebServer.h>
#include<FS.h>


// some var
const char* ssid = "Your WiFi name";
const char* password = "Your WiFi password";
const int ledPin = 2;
String ledState;

// create server
AsyncWebServer server(80);

// led control
String processor(const String& var) {
  Serial.println(var);
  if (var == "STATE") {
    if (digitalRead(ledPin)) {
      ledState = "ON";
    }
    else {
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
}

void setup() {
  // begin
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  //   start
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });


  // Route to load bg.jpg file
  server.on("/bg.jpg", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/bg.jpg", "jpg");
  });

  // Route to load style.css file
  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/styles.css", "text/css");
  });

  // Route to set GPIO to HIGH ?Why they flip
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(ledPin, LOW);
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to set GPIO to LOW Idk
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(ledPin, HIGH);
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.begin();
}
void loop() {
  // nothing here
}


Code 

/***************************************************
 *  BLYNK TEMPLATE INFO
 ***************************************************/
#define BLYNK_TEMPLATE_ID   "TMPL396ujBJnH"
#define BLYNK_TEMPLATE_NAME "Smart Socket 2"
#define BLYNK_AUTH_TOKEN    "Vi0sJxv9D5QOeXdTQY4KraatyMXjbtah"

#define BLYNK_PRINT Serial

/***************************************************
 *  LIBRARIES
 ***************************************************/
#include <WiFi.h>
#include <WebServer.h>
#include <BlynkSimpleEsp32.h>

/***************************************************
 *  WIFI CREDENTIALS
 ***************************************************/
char ssid[] = "Aayan";
char pass[] = "Aayan@11";

/***************************************************
 *  RELAY PINS
 ***************************************************/
#define RELAY1 2
#define RELAY2 18
#define RELAY3 19
#define RELAY4 21

/***************************************************
 *  GLOBAL VARIABLES
 ***************************************************/
WebServer server(80);
bool apMode = false;

int relay1State = 0; // 0=OFF, 1=ON
int relay2State = 0;

/***************************************************
 *  BLYNK VIRTUAL PINS
 ***************************************************/
// Relay 1 (inverted logic)
BLYNK_WRITE(V0) {           
  relay1State = param.asInt();
  digitalWrite(RELAY1, relay1State ? HIGH : LOW);  // Inverted logic
}

// Relay 2 (normal logic)
BLYNK_WRITE(V1) {           
  relay2State = param.asInt();
  digitalWrite(RELAY2, relay2State ? LOW : HIGH);
}

// Relay 3(normal logic)
BLYNK_WRITE(V1) {           
  relay3State = param.asInt();
  digitalWrite(RELAY3, relay3State ? LOW : HIGH);
}

// Relay 2 (normal logic)
BLYNK_WRITE(V1) {           
  relay4State = param.asInt();
  digitalWrite(RELAY4, relay4State ? LOW : HIGH);
}

/***************************************************
 *  SOFT AP WEB PAGE
 ***************************************************/
void handleRoot() {
  String html =
    "<html><body style='text-align:center;font-family:Arial;'>"
    "<h2>Smart Socket (AP Mode)</h2>"
    "<h3>Socket Div01</h3>"
    "<a href='/r1on'><button>ON</button></a>"
    "<a href='/r1off'><button>OFF</button></a>"
    "<h3>Socket Div02</h3>"
    "<a href='/r2on'><button>ON</button></a>"
    "<a href='/r2off'><button>OFF</button></a>"
    "<p>R1: " + String(relay1State ? "ON" : "OFF") + "</p>"
    "<p>R2: " + String(relay2State ? "ON" : "OFF") + "</p>"
    "<p>R3: " + String(relay3State ? "ON" : "OFF") + "</p>"
    "<p>R4: " + String(relay4State ? "ON" : "OFF") + "</p>"
    "</body></html>";

  server.send(200, "text/html", html);
}

/***************************************************
 *  AP BUTTON HANDLERS (SYNC TO BLYNK)
 ***************************************************/
// Relay 1 (inverted)
void r1on() {
  relay1State = 1;
  digitalWrite(RELAY1, HIGH);  // Inverted
  Blynk.virtualWrite(V0, 1);
  server.sendHeader("Location","/");
  server.send(303);
}

void r1off() {
  relay1State = 0;
  digitalWrite(RELAY1, LOW);   // Inverted
  Blynk.virtualWrite(V0, 0);
  server.sendHeader("Location","/");
  server.send(303);
}

// Relay 2 (normal)
void r2on() {
  relay2State = 1;
  digitalWrite(RELAY2, LOW);
  Blynk.virtualWrite(V1, 1);
  server.sendHeader("Location","/");
  server.send(303);
}

void r2off() {
  relay2State = 0;
  digitalWrite(RELAY2, HIGH);
  Blynk.virtualWrite(V1, 0);
  server.sendHeader("Location","/");
  server.send(303);
}
// Relay 3  (normal)
void r1on() {
  relay1State = 1;
  digitalWrite(RELAY3, HIGH);  // Inverted
  Blynk.virtualWrite(V0, 1);
  server.sendHeader("Location","/");
  server.send(303);
}
void r1off() {
  relay1State = 0;
  digitalWrite(RELAY3, LOW);   
  Blynk.virtualWrite(V0, 0);
  server.sendHeader("Location","/");
  server.send(303);
}

//Relay 4(normal)
void r1on() {
  relay1State = 1;
  digitalWrite(RELAY4, HIGH);  // Inverted
  Blynk.virtualWrite(V0, 1);
  server.sendHeader("Location","/");
  server.send(303);
}

void r1off() {
  relay1State = 0;
  digitalWrite(RELAY4, LOW);   
  Blynk.virtualWrite(V0, 0);
  server.sendHeader("Location","/");
  server.send(303);
}

/***************************************************
 *  START SOFT AP MODE
 ***************************************************/
void startAP() {
  apMode = true;
  WiFi.mode(WIFI_AP);
  WiFi.softAP("SmartPlug_AP", "12345678");

  Serial.println("⚠ WiFi Failed → AP Mode");
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/r1on", r1on);
  server.on("/r1off", r1off);
  server.on("/r2on", r2on);
  server.on("/r2off", r2off);
  server.on("/r3on", r2on);
  server.on("/r3off", r2off);
  server.on("/r4on", r2on);
  server.on("/r4off", r2off);
  server.begin();
}

/***************************************************
 *  SETUP
 ***************************************************/
void setup() {
  Serial.begin(115200);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);

  digitalWrite(RELAY1, LOW); // OFF due to inverted logic
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH); // OFF

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  Serial.print("Connecting to WiFi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
    Serial.print(".");
    delay(300);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi Connected");
    Serial.println(WiFi.localIP());
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

    // Initial sync (ESP32 → Blynk)
    Blynk.virtualWrite(V0, relay1State);
    Blynk.virtualWrite(V1, relay2State);
    Blynk.virtualWrite(V12, relay2State);
    Blynk.virtualWrite(V13, relay2State);
  } else {
    startAP();
  }
}

/***************************************************
 *  LOOP
 ***************************************************/
void loop() {
  if (apMode) {
    server.handleClient();
  } else {
    Blynk.run();

    if (WiFi.status() != WL_CONNECTED) {
      startAP();
    }
  }
}

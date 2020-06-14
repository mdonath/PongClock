//
// Board Mgmt: https://dl.espressif.com/dl/package_esp32_index.json
// Board: WEMOS LOLIN32
// Flash Frequency: 80 MHz
// 


#include <WiFi.h>
#include <PubSubClient.h>

#include "UserInterfaceInput.hpp"
#include "ClockDisplay.hpp"
#include "PongAudio.hpp"

#define WIFI_SSID "<<YOUR WIFI SSID>>"
#define WIFI_PASS "<<YOUR WIFI PASSWORD>>"

#define MQTT_HOST "<<YOUR MQTT HOST IP>>"
#define MQTT_USER "<<YOUR MQTT USER>>"
#define MQTT_PASS "<<YOU MQTT PASSWORD>>" 
#define MQTT_VERSION MQTT_VERSION_3_1

#define NTP_ADDRESS  "<<YOUR NTP SERVERNAME>>" // "nl.pool.ntp.org"
#define NTP_OFFSET   1  // In HOURS
#define NTP_INTERVAL 60*60 * 1000    // In miliseconds


WiFiClient espClient;
PubSubClient client(espClient);

UserInterfaceInput* uiInput;
ClockDisplay* clockDisplay;

void setupWifiConnectDirect() {
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to "); Serial.println(WIFI_SSID);
 
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
 
    if ((++i % 16) == 0)
    {
      Serial.println(F(" still trying to connect"));
    }
  }
 
  Serial.print(F("Connected. My IP address is: "));
  Serial.println(WiFi.localIP());
}

void setupSerial() {
  Serial.begin(115200);
  Serial.println();
} 

void setupMQTT() {
  client.setServer(MQTT_HOST, 1883);
  client.setCallback(mqtt_callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("PongClock", MQTT_USER, MQTT_PASS)) {
      Serial.println("connected");
      client.publish("home/pongclock/info", "Pong Clock started!");
    } else {
      Serial.print("failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }
  client.subscribe("home/pongclock/display");
}

void setupTimeWithNTP() {
  configTime(NTP_OFFSET * 3600, 3600, NTP_ADDRESS);
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.print("Datetime: ");
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");Serial.println(topic);

  payload[length] = '\0';
  String message((char *)payload);
  Serial.print("Message: "); Serial.println(message);
  
  if (message == "switch") {
    clockDisplay->changeToNextMode();
  } else if (message.startsWith("scroll")) {
    clockDisplay->scroll(message.substring(7));
  } else {
    bool isFound = clockDisplay->changeToMode(message);
    if (!isFound) {
      Serial.print("Onbekend commando: "); Serial.println(message);
    }
  }

  Serial.println();
  Serial.println("-----------------------");
}


static int mqttTaskCore = 1;

void setup() {
  setupSerial();
  // setupWifiManager();
  setupWifiConnectDirect();
  setupTimeWithNTP();

  uiInput = new UserInterfaceInput();
  uiInput->setup();
  clockDisplay = new ClockDisplay(uiInput);
  clockDisplay->setup();
  //clockDisplay->printVersion();

  setupMQTT();

  xTaskCreatePinnedToCore(
    mqttCoreTask,   /* Function to implement the task */
    "mqttCoreTask", /* Name of the task */
    10000,          /* Stack size in words */
    NULL,           /* Task input parameter */
    0,              /* Priority of the task */
    NULL,           /* Task handle. */
    mqttTaskCore);  /* Core where the task should run */

  Serial.println("Task created...");
}

void mqttCoreTask(void * pvParameters) {
  while (true) {
    client.loop();
  }
}

/**
   Main loop.
*/
void loop() {
  Serial.println("loop(): enter");
  clockDisplay->loop();
  Serial.println("loop(): leave");
}

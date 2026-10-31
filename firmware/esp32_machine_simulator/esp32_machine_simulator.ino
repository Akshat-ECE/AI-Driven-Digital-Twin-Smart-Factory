#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// ==================================================
// 1. WI-FI CONFIGURATION
// ==================================================
const char* ssid = "OPPO K12x 5G";
const char* wifi_password = "u2g4ricn";

// ==================================================
// 2. EMQX CLOUD MQTT CONFIGURATION
// ==================================================
const char* mqtt_server =
    "wdd94841.ala.asia-southeast1.emqxsl.com";

const int mqtt_port = 8883;

const char* mqtt_user = "esp32_machine1";
const char* mqtt_password = "esp32_machine1";

// ==================================================
// 3. MQTT CLIENT
// ==================================================
WiFiClientSecure secureClient;
PubSubClient mqttClient(secureClient);

unsigned long lastPublish = 0;
const unsigned long publishInterval = 3000;

// ==================================================
// 4. WI-FI CONNECTION
// ==================================================
void connectWiFi() {
  Serial.print("Connecting to Wi-Fi");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, wifi_password);

  int attempts = 0;

  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi connected!");
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWi-Fi connection failed.");
  }
}

// ==================================================
// 5. MQTT CONNECTION
// ==================================================
bool connectMQTT() {
  if (mqttClient.connected()) {
    return true;
  }

  if (WiFi.status() != WL_CONNECTED) {
    return false;
  }

  Serial.print("Connecting to EMQX Cloud...");

  String clientId =
      "ESP32_Machine1_" + String((uint32_t)ESP.getEfuseMac(), HEX);

  if (mqttClient.connect(
        clientId.c_str(),
        mqtt_user,
        mqtt_password)) {

    Serial.println("connected!");
    return true;
  }

  Serial.print("failed, rc=");
  Serial.println(mqttClient.state());

  return false;
}

// ==================================================
// 6. PUBLISH ONE NUMERIC MACHINE PARAMETER
// ==================================================
bool publishValue(const char* topic, float value, int decimals = 2) {
  char payload[24];

  snprintf(payload, sizeof(payload), "%.*f", decimals, value);

  bool success = mqttClient.publish(topic, payload);

  if (success) {
    Serial.print(topic);
    Serial.print(" = ");
    Serial.println(payload);
  } else {
    Serial.print("Publish failed: ");
    Serial.println(topic);
  }

  return success;
}

// ==================================================
// 7. SIMULATED MACHINE TELEMETRY
// ==================================================
void publishMachineData() {
  // Each cycle lasts 3 seconds.
  // This is simulated data, not physical sensor readings.
  static int cycle = 0;

  float temperature;
  float pressure;
  float motorSpeed;
  float vibration;
  float current;
  float voltage;
  float humidity;

  // Cycles 8 and 9: simulated fault
  // Cycles 0 and 1: simulated recovery
  // Cycles 5 and 6: simulated warning
  // Other cycles: normal

  if (cycle == 8 || cycle == 9) {
    temperature = random(451, 551) / 10.0;
    pressure    = random(111, 126);
    motorSpeed  = random(1100, 1350);
    vibration   = random(28, 51) / 10.0;
    current     = random(121, 151) / 10.0;
    voltage     = random(350, 452);
    humidity    = random(85, 96);

    Serial.println("\n*** SIMULATED FAULT ***");

  } else if (cycle == 5 || cycle == 6) {
    temperature = random(400, 451) / 10.0;
    pressure    = random(103, 111);
    motorSpeed  = random(1350, 1420);
    vibration   = random(16, 28) / 10.0;
    current     = random(105, 121) / 10.0;
    voltage     = random(380, 422);
    humidity    = random(70, 86);

    Serial.println("\n*** SIMULATED WARNING ***");

  } else if (cycle == 0 || cycle == 1) {
    temperature = random(350, 401) / 10.0;
    pressure    = random(101, 106);
    motorSpeed  = random(1380, 1460);
    vibration   = random(10, 21) / 10.0;
    current     = random(90, 106) / 10.0;
    voltage     = random(390, 412);
    humidity    = random(50, 76);

    Serial.println("\n*** SIMULATED RECOVERY ***");

  } else {
    temperature = random(250, 401) / 10.0;
    pressure    = random(980, 1021) / 10.0;
    motorSpeed  = random(1420, 1481);
    vibration   = random(30, 151) / 100.0;
    current     = random(750, 1001) / 100.0;
    voltage     = random(390, 412);
    humidity    = random(400, 701) / 10.0;

    Serial.println("\n*** MACHINE NORMAL ***");
  }

  // Topics must match the existing EMQX rule and Render receiver.
  publishValue("factory/machine1/temperature", temperature, 1);
  publishValue("factory/machine1/pressure", pressure, 1);
  publishValue("factory/machine1/motor_speed", motorSpeed, 0);
  publishValue("factory/machine1/vibration", vibration, 2);
  publishValue("factory/machine1/current", current, 2);
  publishValue("factory/machine1/voltage", voltage, 1);
  publishValue("factory/machine1/humidity", humidity, 1);

  Serial.println("------------------------------");

  cycle = (cycle + 1) % 10;
}

// ==================================================
// 8. SETUP
// ==================================================
void setup() {
  Serial.begin(115200);
  delay(1000);

  randomSeed(micros());

  // Prototype configuration: encrypts MQTT traffic but
  // does not verify the broker's certificate.
  secureClient.setInsecure();

  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setKeepAlive(30);
  mqttClient.setSocketTimeout(10);

  connectWiFi();
}

// ==================================================
// 9. MAIN LOOP
// ==================================================
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected. Reconnecting...");
    WiFi.disconnect();
    connectWiFi();
    delay(1000);
    return;
  }

  if (!mqttClient.connected()) {
    if (!connectMQTT()) {
      delay(5000);
      return;
    }
  }

  mqttClient.loop();

  if (millis() - lastPublish >= publishInterval) {
    lastPublish = millis();
    publishMachineData();
  }
}

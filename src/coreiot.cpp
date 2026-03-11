#include "coreiot.h"

// ----------- CONFIGURE THESE! -----------
const char* coreIOT_Server = "app.coreiot.io";  
const char* coreIOT_Token = "aw84kht70nil2989jmde";   // Device Access Token
const int   mqttPort = 1883;
// ----------------------------------------

WiFiClient espClient;
PubSubClient client(espClient);
#define LED_PIN 2


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect (username=token, password=empty)
    //if (client.connect("ESP32Client", coreIOT_Token, NULL)) {
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
        
      Serial.println("connected to CoreIOT Server!");
      client.subscribe("v1/devices/me/rpc/request/+");
      Serial.println("Subscribed to v1/devices/me/rpc/request/+");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


// void callback(char* topic, byte* payload, unsigned int length) {
//   Serial.print("Message arrived [");
//   Serial.print(topic);
//   Serial.println("] ");

//   // Allocate a temporary buffer for the message
//   char message[length + 1];
//   memcpy(message, payload, length);
//   message[length] = '\0';
//   Serial.print("Payload: ");
//   Serial.println(message);

//   // Parse JSON
//   StaticJsonDocument<256> doc;
//   DeserializationError error = deserializeJson(doc, message);

//   if (error) {
//     Serial.print("deserializeJson() failed: ");
//     Serial.println(error.c_str());
//     return;
//   }

//   const char* method = doc["method"];
//   if (strcmp(method, "setStateLED") == 0) {
//     // Check params type (could be boolean, int, or string according to your RPC)
//     // Example: {"method": "setValueLED", "params": "ON"}
//     const char* params = doc["params"];

//     if (strcmp(params, "ON") == 0) {
//       digitalWrite(LED_PIN, HIGH);
//       Serial.println("Device turned ON.");
//       //TODO

//     } else {   
//       digitalWrite(LED_PIN, LOW);

//       Serial.println("Device turned OFF.");
//       //TODO

//     }
//   } else {
//     Serial.print("Unknown method: ");
//     Serial.println(method);
//   }
// }
//bool ledState = false;

void callback(char* topic, byte* payload, unsigned int length) {

  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';

  Serial.println(message);

  StaticJsonDocument<256> doc;
  deserializeJson(doc, message);

  const char* method = doc["method"];

  if (strcmp(method, "setValueLED") == 0) {

    bool state = doc["params"];

  //   if (state) {
  //     digitalWrite(LED_PIN, HIGH);
  //     ledState = true;
  //     Serial.println("LED ON");
  //   } else {
  //     digitalWrite(LED_PIN, LOW);
  //     ledState = false;
  //     Serial.println("LED OFF");
  //   }
  // }

  // if (strcmp(method, "getValueLED") == 0) {

  //   String response = ledState ? "true" : "false";

  //   client.publish("v1/devices/me/rpc/response/1", response.c_str());
  }
}

void setup_coreiot(){
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  //Serial.print("Connecting to WiFi...");
  //WiFi.begin(wifi_ssid, wifi_password);
  //while (WiFi.status() != WL_CONNECTED) {
  
  // while (isWifiConnected == false) {
  //   delay(500);
  //   Serial.print(".");
  // }

  while(1){
    if (xSemaphoreTake(xBinarySemaphoreInternet, portMAX_DELAY)) {
      Serial.println("Connected to WiFi");
      break;
    }
    delay(500);
    Serial.print(".");
  }


  Serial.println(" Connected!");

  client.setServer(CORE_IOT_SERVER.c_str(), CORE_IOT_PORT.toInt());
  client.setCallback(callback);

}

// void coreiot_task(void *pvParameters){

//     setup_coreiot();

//     while(1){

//         if (!client.connected()) {
//             reconnect();
//         }
//         client.loop();

//         // Sample payload, publish to 'v1/devices/me/telemetry'
//         String payload = "{\"temperature\":" + String(glob_temperature) +  ",\"humidity\":" + String(glob_humidity) + "}";
        
//         client.publish("v1/devices/me/telemetry", payload.c_str());


        
//         Serial.println("Published payload: " + payload);
//         vTaskDelay(10000);  // Publish every 10 seconds
//     }
// }
void coreiot_task(void *pvParameters)
{
    SensorData_t recvData;

    setup_coreiot();

    while (1)
    {
        if (!client.connected()) {
            reconnect();
        }
        client.loop();

        // Chờ dữ liệu mới từ sensor task
        if (xQueueReceive(sensorDataQueue, &recvData, portMAX_DELAY) == pdPASS)
        {
            String payload = "{";
            payload += "\"temperature\":" + String(recvData.temperature);
            payload += ",\"humidity\":" + String(recvData.humidity);
            payload += "}";

            client.publish("v1/devices/me/telemetry", payload.c_str());

            Serial.println("Published payload: " + payload);
        }

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

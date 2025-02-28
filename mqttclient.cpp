#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "QA-TEAM-1-2G";
const char* password = "ultimateguru12";
const char* mqtt_server = "192.168.30.38";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  reconnect();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Subscribe to the topic where the LED control messages will be received
      client.subscribe("led_control");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Check the incoming message
  if (strcmp(topic, "led_control") == 0) {
    // Convert payload to integer
    int command = atoi((char *)payload);

    // Control the LED based on the command received
    if (command == 1) {
      digitalWrite(LED_PIN, HIGH); // Turn LED ON
    } else {
      digitalWrite(LED_PIN, LOW);  // Turn LED OFF
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

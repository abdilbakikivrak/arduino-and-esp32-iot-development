#include <WiFi.h>
#include <PubSubClient.h>

//TODO: ESP32 MQTT user config
const char* ssid = "kivrak_system"; // Wifi SSID
const char* password = "adminkivrak"; // Wifi Password
const char* username = "adminkivrak1"; // my AskSensors username
const char* subTopic = "actuator/adminkivrak1/Fj5dv7xKXOtMLr2i7gEkpMW8n43yjiZw"; // actuator/username/apiKeyOut
const int LED_pin = 2; // LEd pin

//AskSensors MQTT config
const char* mqtt_server = "mqtt.asksensors.com";
unsigned int mqtt_port = 1883;

WiFiClient askClient;
PubSubClient client(askClient);

void setup() {
Serial.begin(115200);
Serial.println("*****************************************************");
Serial.println("********** Program Start : ESP32 controls LED with AskSensors over MQTT");
Serial.println("Set LED as output");
pinMode(LED_pin, OUTPUT); // set led as output

Serial.print("********** connecting to WIFI : ");
Serial.println(ssid);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("->WiFi connected");
Serial.println("->IP address: ");
Serial.println(WiFi.localIP());

client.setServer(mqtt_server, mqtt_port);
client.setCallback(callback);

if (!client.connected()) 
reconnect();
Serial.print("********** Subscribe to AskSensors actuator topic:");
Serial.print(subTopic);
// susbscribe
client.subscribe(subTopic);
}

void loop() {
client.loop();
}


void callback(char* topic, byte* payload, unsigned int length) {

Serial.print("Command teceived from AskSensors[");
Serial.print(topic);
Serial.print("] ");
for (int i = 0; i < length; i++) {
Serial.print((char)payload[i]);
}

Serial.println("********** Parse Actuator command");
/*
if(strcmp((char *)payload, (char*) "module1=1" )== 0){ 
  String command = String((char*)payload);  
digitalWrite(LED_pin, command.toInt());
Serial.println("LED is ON");

} else{
digitalWrite(LED_pin,command.toInt());
Serial.println("LED is OFF");
}
}*/
  String command = String((char*)payload); // add this line
  
  digitalWrite(LED_pin, !command.toInt());
  
  if(command.toInt() == HIGH){
    Serial.println("LED is ON");
  }
  else{
    Serial.println("LED is OFF");
  }
}

void reconnect() {
// Loop until we're reconnected
while (!client.connected()) {
Serial.print("********** Attempting MQTT connection...");
// Attempt to connect
if (client.connect("ESP32Client", username, "")) { 
Serial.println("-> MQTT client connected");
} else {
Serial.print("failed, rc=");
Serial.print(client.state());
Serial.println("-> try again in 5 seconds");
// Wait 5 seconds before retrying
delay(5000);
}
}
}